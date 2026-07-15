#pragma once

#include "CoreMinimal.h"
#include "System/QuestObjective.h"
#include "QuestObjective_DefinitionOfDone.generated.h"

/**
 * Threshold ("Definition of Done") composite - owns a list of child
 * objectives and resolves based on counts, not strict AND/OR:
 *   - Done once at least RequiredDoneCount children are Done.
 *   - Failed once at least RequiredFailCount children are Failed.
 *
 * Useful for rules that don't fit clean AND/OR logic - e.g. "complete
 * 3 of these 5 side objectives" or "fail once 2 defenders die, but
 * succeed once 3 waves are cleared regardless of the rest".
 *
 * If both thresholds are met on the same evaluation, Failed takes
 * priority - see GetState_Implementation.
 */
UCLASS(meta = (DisplayName = "Definition Of Done"))
class QUESTCORERUNTIME_API UQuestObjective_DefinitionOfDone : public UQuestObjective
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Instanced, Category = "Quest")
    TArray<TObjectPtr<UQuestObjective>> ChildObjectives;
    // Number of children that must be Done for this objective to be Done.
    UPROPERTY(EditAnywhere, Category = "Quest", meta = (ClampMin = "0"))
    int32 RequiredDoneCount = 1;
    // Number of children that must be Failed for this objective to be
    // Failed. 0 disables the fail threshold entirely - failures are ignored.
    UPROPERTY(EditAnywhere, Category = "Quest", meta = (ClampMin = "0"))
    int32 RequiredFailCount = 0;

public:
    virtual void Construction(UQuestComponent *Quest, UQuestDefinition *Defination) override
    {
        for (UQuestObjective *Child : ChildObjectives)
        {
            if (Child)
            {
                Child->Construction(Quest, Defination);
            }
        }
    }

protected:
    virtual void Begin_Implementation() override
    {
        for (UQuestObjective *Child : ChildObjectives)
        {
            if (Child)
            {
                Child->Begin();
            }
        }
    }
    virtual void End_Implementation() override
    {
        for (UQuestObjective *Child : ChildObjectives)
        {
            if (Child)
            {
                Child->End();
            }
        }
    }
    virtual EQuestObjectiveState GetState_Implementation() const override
    {
        int32 DoneCount = 0;
        int32 FailCount = 0;

        for (const UQuestObjective *Child : ChildObjectives)
        {
            if (!Child)
            {
                continue;
            }

            switch (Child->GetState())
            {
            case EQuestObjectiveState::Done:
                DoneCount++;
                break;
            case EQuestObjectiveState::Failed:
                FailCount++;
                break;
            default:
                break;
            }
        }

        // Fail threshold takes priority if both are met on the same
        // evaluation - failure is treated as the more decisive outcome.
        if (RequiredFailCount > 0 && FailCount >= RequiredFailCount)
        {
            return EQuestObjectiveState::Failed;
        }

        if (DoneCount >= RequiredDoneCount)
        {
            return EQuestObjectiveState::Done;
        }

        return EQuestObjectiveState::InProgress;
    }
    virtual float GetProgress_Implementation() const override
    {
        if (RequiredDoneCount <= 0)
        {
            return 1.f;
        }

        int32 DoneCount = 0;
        for (const UQuestObjective *Child : ChildObjectives)
        {
            if (Child && Child->GetState() == EQuestObjectiveState::Done)
            {
                DoneCount++;
            }
        }

        return FMath::Clamp(static_cast<float>(DoneCount) / static_cast<float>(RequiredDoneCount), 0.f, 1.f);
    }
    virtual FString GetObjectiveDescription_Implementation() const override { return TEXT("Definition Of Done"); }
};