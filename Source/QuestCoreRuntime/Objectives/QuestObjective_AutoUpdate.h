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
    FTimerHandle TimerHandle;

private:
    void HandleTimerTick()
    {
        UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;

        const UQuestDefinition *MyDefination = GetQuestComponent()->GetDefinition();
        if (!Subsystem || !MyDefination)
        {
            return;
        }

        if (UQuestComponent *Quest = Subsystem->FindQuestByDefinition(MyDefination))
        {
            Quest->UpdateQuest();
        }
    }

protected:
    virtual void Begin_Implementation() override
    {
        const AActor *Owner = GetOwner();
        World->GetTimerManager().SetTimer(TimerHandle, this, &UQuestObjective_AutoUpdate::HandleTimerTick, Duration, /*bLoop=*/true);
    }
    virtual void End_Implementation() override
    {
        const AActor *Owner = GetOwner();
        World->GetTimerManager().ClearTimer(TimerHandle);
    }
    virtual EQuestObjectiveState GetState_Implementation() const override { return EQuestObjectiveState::Done; }
    virtual float GetProgress_Implementation() const override { return 1.f; }
    virtual FString GetObjectiveDescription_Implementation() const override { return TEXT("Auto Update"); }
};