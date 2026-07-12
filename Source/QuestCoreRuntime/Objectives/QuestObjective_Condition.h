#pragma once

#include "CoreMinimal.h"
#include "System/QuestObjective.h"
#include "QuestObjective_Condition.generated.h"

/**
 * Objective that resolves by checking a condition live.
 * Cheap to evaluate, no binding needed. Only ever reads as
 * InProgress or Done - use the Event objective if the objective
 * needs to be able to Fail.
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class QUESTCORERUNTIME_API UQuestObjective_Condition : public UQuestObjective
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Quest|Objective")
	bool CheckCondition() const;
	virtual bool CheckCondition_Implementation() const { return false; }

	virtual EQuestObjectiveState GetState_Implementation() const override
	{
		return CheckCondition() ? EQuestObjectiveState::Done : EQuestObjectiveState::InProgress;
	}
};
