# QuestCore

<p align="center">
  <img src="Resources/Icon128.png" alt="QuestCore Icon" />
</p>

<p align="center">
  <b>A lightweight Unreal Engine C++ plugin for fast quest-system prototyping</b><br/>
   Composable set of building blocks: *Quests* are `ActorComponent`s, **Objectives** are Command Pattern objects, and everything is driven by explicit `UpdateQuest()` calls instead of ticking.
</p>

<p align="center">
  <img alt="Unreal Engine" src="https://img.shields.io/badge/Unreal%20Engine-5.6%2B-313131?logo=unrealengine" />
  <img alt="License" src="https://img.shields.io/badge/license-MIT-green" />
  <img alt="Platform" src="https://img.shields.io/badge/platform-Runtime%20Plugin-blue" />
</p>

---


## Why QuestCore

- **No tick, anywhere.** Objectives resolve by polling (checked when `UpdateQuest()` runs) or by binding to gameplay events — never a per-frame `Tick()`.
- **Command Pattern objectives.** Every objective is a small, self-contained object with the same four hooks (`Begin`, `End`, `GetState`, `GetProgress`), so new objective types are cheap to add and Quest never needs to know how one resolves internally.
- **Data-asset quest identity.** Quests are identified by a `UQuestDefinition` asset (with its own right-click **Create → Quest → Quest Definition** entry), not a hand-typed string.
- **Built-in save/load**, project-wide config via **Project Settings → Game → Quest Core**, and a small function library for Blueprint-side quest control.

## Installation

1. Copy the `QuestCore` folder into your project's `Plugins/` directory.
2. Enable **QuestCore** in Edit → Plugins.
3. Regenerate project files and build.

## Quick Start

1. **Create a Quest Definition asset** — right-click in the Content Browser (Create QuestDefinition(DataAsset)). Give it a `QuestId` (auto-fills from the asset name) and optionally a `DisplayName`/`Description`.

2. **Add a `QuestComponent`** to an actor (an NPC, a manager actor, whatever owns the quest), assign the Quest Definition, and add one or more Objectives to its instanced `Objectives` array.

3. **Drive it from gameplay code:**

```cpp
UQuestSubsystem *Subsystem = GetWorld()->GetSubsystem<UQuestSubsystem>();

// Activate a quest by its definition asset
Subsystem->ActivateQuest(MyQuestDefinition);

// Or by id
Subsystem->ActivateQuestById(TEXT("MyFirstQuest"));

// Re-evaluate whenever something relevant happens
Quest->UpdateQuest();

// Check progress/status
const float Progress = Quest->GetProgress();
const bool bDone = Subsystem->IsQuestCompletedById(TEXT("MyFirstQuest"));
```

Event-driven objectives (kill counts, item collection, dialogue) resolve on their own once the right event fires and call `UpdateQuest()` internally — no manual polling loop needed for those.

## Architecture

```
UQuestObjective (abstract)
 ├── Begin()   - called once the objective becomes active
 ├── End()                      - cleanup, always called (success or fail)
 ├── GetOwner()     - return owner actor has Questcomponent
 ├── GetDefination()    - return Defination asset
 ├── GetState() -> EQuestObjectiveState   (InProgress / Done / Failed / Canceld)
 └── GetProgress() -> float     (0-1, optional, for UI)

UQuestComponent : UActorComponent
 ├── QuestDefinition            - identity (UQuestDefinition data asset)
 ├── Objectives[]               - the objectives that make up this quest
 ├── Prerequisites[]            - UQuestPrerequisite command objects gating availability
 └── UpdateQuest()              - re-checks all objectives, advances/completes/fails

UQuestSubsystem : UWorldSubsystem
 ├── Registers/unregisters quests as they BeginPlay/EndPlay
 ├── GetAvailableQuests() / GetActiveQuests()
 ├── FindQuestById() / FindQuestByDefinition()
 ├── Save/Load via UQuestSaveGame (QuestId -> EQuestState map)
 └── OnQuestStateChanged delegate

```

A quest completes once **every** objective in its `Objectives` array reports `Done`; if any reports `Failed`, the quest fails. Objectives don't need to be authored one-at-a-time — `Composite` and `DefinitionOfDone` (below) let a single "objective slot" represent AND/OR/threshold logic over several sub-objectives.

## Runtime Flow
 
**1. Startup (per world)**
`UQuestSubsystem::Initialize()` runs first, before any quest exists yet. It calls `LoadQuestData()`, which reads the configured save slot (if one exists) into `PendingLoadedStates` — a `QuestId -> EQuestState` map staged in memory, not applied to anything yet, since no `QuestComponent` has registered at this point.
 
**2. A quest comes online**
When a `QuestComponent`'s owning actor spawns, `BeginPlay()`:
- Calls `Construction(this, QuestDefinition)` on every objective in `Objectives` — this only wires up each objective's back-reference to its owning quest and definition (`MyQuest` / `MyDefination`). It does **not** start the objective; that's `Begin()`'s job, later.
- Registers with the subsystem via `RegisterQuest()`, which rejects the quest (and screen-logs) if another registered quest already shares its `QuestId`.
- If registration succeeds and a saved state exists for this `QuestId` in `PendingLoadedStates`, `ApplyLoadedState()` restores it immediately — `Completed`/`Failed` just set `State` directly (nothing to resume), `Active` re-enters through `SetState(Active)`, which re-`Begin()`s every objective so event-driven ones can rebind.
- Only then, if nothing was loaded and `bAutoActive` is set, does it call `ActivateQuest()` itself.
**3. Activation**
`ActivateQuest()` checks it isn't already `Active`, then `ArePrerequisitesSatisfied()` — which checks two independent things: every `UQuestDefinition` in `QuestDefinition->QuestDependencies` must already be `Completed` (cross-quest dependency chain), **and** every `UQuestPrerequisite` in the component's own `Prerequisites` list must pass. If both hold, `SetState(Active)` runs, which broadcasts the subsystem-level `OnQuestStateChanged`, then calls `Begin()` on every objective — this is the point objectives actually start polling/binding.
 
**4. Resolving**
Something calls `UpdateQuest()` — an objective calling its own protected `UpdateQuest()` helper after resolving itself, an `AutoUpdate` objective's timer, or external gameplay code. `UpdateQuest()` reads every objective's `GetState()`: any `Failed` short-circuits to `SetState(Failed)`; all `Done` triggers `SetState(Completed)`; otherwise it just broadcasts `OnQuestUpdated` and waits.
 
**5. Resolution**
`SetState(Completed)` / `SetState(Failed)` both: broadcast the subsystem-level `OnQuestStateChanged`, call `End()` on every objective (cleanup — unbind events, clear timers), broadcast the per-quest `OnQuestCompleted`/`OnQuestFailed`, then — if `QuestDefinition` is set — walk `QuestDefinition->OnCompleted` (or `OnFailed`) and call `ActivateQuest()` on whichever registered quest matches each listed definition, chaining straight into the next quest with no extra wiring needed. If `QuestDefinition->bAutoSave` is set, this also triggers `SaveQuestData()` right here. If `bAutoDestroy` is set, the owning actor is destroyed.
 
**6. Teardown**
`EndPlay()` calls `DeactivateQuest()` (which, if the quest was `Active`, runs `SetState(NotStarted)` → `End()`s every objective) and then `UnregisterQuest()`.
 
**7. Editor-time validation (separate from the runtime flow above)**
`PostEditChangeProperty()` fires on any property edit to the component in the editor. If `QuestDefinition` is assigned, it cross-checks four independent things — each gated by its own `bOverride*` flag on the definition (`bOverrideAutoActive`, `bOverrideAutoDestroy`, `bOverrideObjective`, `bOverridePrerequisit`) — and screen-logs a red message for any mismatch: `bAutoActive`/`bAutoDestroy` not matching the definition's expected value, or the `Objectives`/`Prerequisites` arrays not matching the definition's `ObjectiveClasses`/`PrerequisitClasses` in count or per-index class. Nothing here blocks saving or PIE — it's advisory only.

## Built-in Objectives

| Class | Resolves by | Notes |
|---|---|---|
| `ReachLocation` | Polling | Distance check between two resolvable actors (owner, a player pawn by index, or a fixed target actor) |
| `KillActors` | Event | N kills of a target class, filtered to the quest owner |
| `CollectItems` | Event | Tag-based (`FName ItemId`) so it's reusable across item types |
| `TalkToActor` | Event | One-shot dialogue completion |
| `Timer` | `TimerManager` | Single-shot wait, not a per-frame tick |
| `Composite` | Aggregates children | `RequireAll` (AND) or `RequireAny` (OR) over a list of child objectives |
| `DefinitionOfDone` | Aggregates children | Threshold-based: Done once *X* children are Done, Failed once *Y* are Failed |
| `If` | Watches another objective | Resolves Done once a target objective's state (Equal/NotEqual) matches a comparison value — for branching quest logic |
| `AutoUpdate` | Looping timer | Utility objective; periodically finds its own quest via the subsystem and calls `UpdateQuest()`. Always reports Done so it never blocks quest completion — it's a driver, not a gameplay condition |

All of these are `Blueprintable` — override `Begin`/`End`/`GetState`/`GetProgress` in Blueprint to add project-specific objectives without touching C++.

## Save / Load

`UQuestSubsystem` loads automatically on `Initialize()` if a save file exists for the configured slot, and stages the loaded state until each quest actually registers (since quests register later, in `BeginPlay`). Call `SaveQuestData()` whenever your game already saves.

The save format (`UQuestSaveGame`) is just `TMap<FName /*QuestId*/, EQuestState>`. Point **Project Settings → Game → Quest Core → Save Game Class** at your own `UQuestSaveGame` subclass to persist additional project-specific data without touching `QuestSubsystem`.

## Project Settings

Configurable under **Project Settings → Game → Quest Core**:

- `Save Slot Name` / `Save User Index`
- `Save Game Class` — which `USaveGame` subclass to use

## Status

Alpha. Built for fast iteration during prototyping, not yet battle-tested at production scale. Things intentionally *not* included yet: per-objective progress persistence (save/load only tracks top-level quest state), quest ordering/branching graphs, and networking/replication.