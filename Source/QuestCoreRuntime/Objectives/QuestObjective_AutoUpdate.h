#pragma once

#include "CoreMinimal.h"
#include "System/QuestObjective.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "SubSystem/QuestSubsystem.h"
#include "System/QuestComponent.h"
#include "QuestObjective_AutoUpdate.generated.h"

/**
 * Utility objective - not a real gameplay condition. Starts a looping
 * timer on Begin(); each tick it finds its own quest via the
 * QuestSubsystem and calls UpdateQuest() on it. Useful for objectives
 * that resolve from external state the quest system doesn't otherwise
 * get notified about (e.g. a Condition-style objective with nothing
 * pushing updates), without falling back to a per-frame Tick.
 *
 * Always reports Done - it must NOT count toward the quest's AND check
 * across objectives (see UQuestComponent::UpdateQuest, which requires
 * every objective to be Done). It keeps looping regardless of its own
 * reported state; it only stops when the owning Quest calls End() on
 * it (quest completes, fails, or is deactivated).
 */
UCLASS(meta = (DisplayName = "Auto Update"))
class QUESTCORERUNTIME_API UQuestObjective_AutoUpdate : public UQuestObjective
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Quest")
    float Duration = 1.f;

private:
    UPROPERTY()
    TWeakObjectPtr<AActor> OwnerActor;
    UPROPERTY()
    TWeakObjectPtr<UQuestDefinition> CachedDefinition;
    FTimerHandle TimerHandle;

private:
    void HandleTimerTick()
    {
        UWorld *World = OwnerActor.IsValid() ? OwnerActor->GetWorld() : nullptr;
        UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;

        if (!Subsystem || !CachedDefinition.IsValid())
        {
            return;
        }

        if (UQuestComponent *Quest = Subsystem->FindQuestByDefinition(CachedDefinition.Get()))
        {
            Quest->UpdateQuest();
        }
    }

protected:
    virtual void Begin_Implementation(AActor *Owner, UQuestDefinition *Defination) override
    {
        OwnerActor = Owner;
        CachedDefinition = Defination;

        if (UWorld *World = Owner ? Owner->GetWorld() : nullptr)
        {
            World->GetTimerManager().SetTimer(TimerHandle, this, &UQuestObjective_AutoUpdate::HandleTimerTick, Duration, /*bLoop=*/true);
        }
    }
    virtual void End_Implementation() override
    {
        if (UWorld *World = OwnerActor.IsValid() ? OwnerActor->GetWorld() : nullptr)
        {
            World->GetTimerManager().ClearTimer(TimerHandle);
        }
    }
    virtual EQuestObjectiveState GetState_Implementation() const override { return EQuestObjectiveState::Done; }
    virtual float GetProgress_Implementation() const override { return 1.f; }
    virtual FString GetObjectiveDescription_Implementation() const override { return TEXT("Auto Update"); }
};