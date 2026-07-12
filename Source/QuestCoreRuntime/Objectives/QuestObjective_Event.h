#pragma once

#include "CoreMinimal.h"
#include "System/QuestObjective.h"
#include "QuestObjective_Event.generated.h"

/**
 * Objective that resolves by reacting to a gameplay event instead
 * of being polled. Bind in Begin(), unbind in End(). The event
 * callback (implemented by children) calls NotifyDone()/NotifyFailed()
 * to cache the result - GetState() just returns the cached value,
 * so the Quest reads it the exact same way as any other objective.
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class QUESTCORERUNTIME_API UQuestObjective_Event : public UQuestObjective
{
	GENERATED_BODY()

public:
	virtual EQuestObjectiveState GetState_Implementation() const override { return CachedState; }

protected:
	// Call from your bound event handler once the objective succeeds.
	UFUNCTION(BlueprintCallable, Category = "Quest|Objective")
	void NotifyDone();

	// Call from your bound event handler if the objective can no longer succeed.
	UFUNCTION(BlueprintCallable, Category = "Quest|Objective")
	void NotifyFailed();

private:
	UPROPERTY(VisibleInstanceOnly, Category = "Quest|Objective")
	EQuestObjectiveState CachedState = EQuestObjectiveState::InProgress;
};
