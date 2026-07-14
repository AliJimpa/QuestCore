#pragma once

#include "CoreMinimal.h"
#include "System/QuestObjective.h"
#include "QuestObjective_If.generated.h"

UENUM(BlueprintType)
enum class EQuestStateComparisonOp : uint8
{
    Equal,
    NotEqual
};

/**
 * Watches another objective (TargetObjective) and resolves Done once
 * its state matches (or doesn't match) CompareToState. Lets you branch
 * quest logic off an objective's state without restructuring order
 * groups - e.g. "Done once the KillBoss objective is Failed" to route
 * a quest down an alternate path.
 *
 * Owns TargetObjective's lifecycle - Begin()/End() are forwarded to it,
 * same as Composite - so it can be used standalone without TargetObjective
 * needing to be a sibling entry elsewhere.
 */
UCLASS(meta = (DisplayName = "If"))
class QUESTCORERUNTIME_API UQuestObjective_If : public UQuestObjective
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Instanced, Category = "Quest")
    TObjectPtr<UQuestObjective> TargetObjective;
    UPROPERTY(EditAnywhere, Category = "Quest")
    EQuestStateComparisonOp Operator = EQuestStateComparisonOp::Equal;
    UPROPERTY(EditAnywhere, Category = "Quest")
    EQuestObjectiveState CompareToState = EQuestObjectiveState::Done;

protected:
    virtual void Construction(AActor *Owner, UQuestDefinition *Defination) override
    {
        Super::Construction(Owner, Defination);
        if (TargetObjective)
        {
            TargetObjective->Construction(Owner, Defination);
        }
    }

public:
    virtual void Begin_Implementation() override
    {
        if (TargetObjective)
        {
            TargetObjective->Begin();
        }
    }
    virtual void End_Implementation() override
    {
        if (TargetObjective)
        {
            TargetObjective->End();
        }
    }
    virtual EQuestObjectiveState GetState_Implementation() const override
    {
        if (!TargetObjective)
        {
            return EQuestObjectiveState::Done;
        }

        const EQuestObjectiveState TargetState = TargetObjective->GetState();
        const bool bConditionMet = (Operator == EQuestStateComparisonOp::Equal)
                                       ? (TargetState == CompareToState)
                                       : (TargetState != CompareToState);

        return bConditionMet ? EQuestObjectiveState::Done : EQuestObjectiveState::InProgress;
    }
    virtual float GetProgress_Implementation() const override
    {
        return TargetObjective ? TargetObjective->GetProgress() : 1.0f;
    }
    virtual FString GetObjectiveDescription_Implementation() const override { return TEXT("If Statement"); }
};