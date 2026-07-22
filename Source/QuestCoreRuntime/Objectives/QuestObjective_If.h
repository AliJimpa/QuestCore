#pragma once

#include "CoreMinimal.h"
#include "System/QuestObjective.h"
#include "System/QuestPrerequisite.h"
#include "QuestObjective_If.generated.h"

UCLASS(meta = (DisplayName = "IF"))
class QUESTCORERUNTIME_API UQuestObjective_If : public UQuestObjective
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Instanced, Category = "Quest")
    TObjectPtr<UQuestPrerequisite> Condition;
    UPROPERTY(EditAnywhere, Instanced, Category = "Quest")
    TObjectPtr<UQuestObjective> True;
    UPROPERTY(EditAnywhere, Instanced, Category = "Quest")
    TObjectPtr<UQuestObjective> False;

protected:
    virtual void Construction(UQuestComponent *Quest) override
    {
        Super::Construction(Quest);
        if (True)
        {
            True->Construction(Quest);
        }
        if (False)
        {
            False->Construction(Quest);
        }
    }

public:
    virtual void Begin_Implementation() override
    {
        if (True)
        {
            True->Begin();
        }
        if (False)
        {
            False->Begin();
        }
    }
    virtual void End_Implementation() override
    {
        if (True)
        {
            True->End();
        }
        if (False)
        {
            False->End();
        }
    }
    virtual EQuestObjectiveState GetState_Implementation() const override
    {
        if (!Condition)
        {
            return EQuestObjectiveState::Done;
        }
        if (Condition->IsSatisfied(GetQuestComponent()))
        {
            if (True)
            {
                return True->GetState();
            }
            else
            {
                return EQuestObjectiveState::Done;
            }
        }
        else
        {
            if (False)
            {
                return False->GetState();
            }
            else
            {
                return EQuestObjectiveState::Done;
            }
        }
    }
    virtual float GetProgress_Implementation() const override
    {
        if (!Condition)
        {
            return 1;
        }
        if (Condition->IsSatisfied(GetQuestComponent()))
        {
            if (True)
            {
                return True->GetProgress();
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if (False)
            {
                return False->GetProgress();
            }
            else
            {
                return 1;
            }
        }
    }
    virtual FString GetObjectiveDescription_Implementation() const override
    {
        if (Condition != nullptr)
        {
            if (Condition->IsSatisfied(GetQuestComponent()))
            {
                if (True)
                {
                    return True->GetObjectiveDescription();
                }
            }
            else
            {
                if (False)
                {
                    return False->GetObjectiveDescription();
                }
            }
        }

        return TEXT("If Statement");
    }
};