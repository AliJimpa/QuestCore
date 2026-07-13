#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestObjective.generated.h"

UENUM(BlueprintType)
enum class EQuestObjectiveState : uint8
{
	InProgress,
	Done,
	Failed,
	Canceld,
};

/**
 * Base class for all quest objectives (Command Pattern).
 * Quest only ever talks to this interface - it never knows
 * whether an objective resolves by polling or by event.
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced, CollapseCategories)
class QUESTCORERUNTIME_API UQuestObjective : public UObject
{
	GENERATED_BODY()

public:
	// Called by the owning Quest when this objective becomes active
	UFUNCTION(BlueprintNativeEvent, Category = "Quest|Objective")
	void Begin(AActor *Owner, UQuestDefinition *Defination);
	virtual void Begin_Implementation(AActor *Owner, UQuestDefinition *Defination) {}

	// Called by the owning Quest when this objective is done resolving
	// (Done or Failed). Always called - use it for cleanup (unbinding etc).
	UFUNCTION(BlueprintNativeEvent, Category = "Quest|Objective")
	void End();
	virtual void End_Implementation() {}

	// Single source of truth the Quest reads on UpdateQuest().
	UFUNCTION(BlueprintNativeEvent, Category = "Quest|Objective")
	EQuestObjectiveState GetState() const;
	virtual EQuestObjectiveState GetState_Implementation() const { return EQuestObjectiveState::InProgress; }

	// Optional 0-1 progress value for UI.
	UFUNCTION(BlueprintNativeEvent, Category = "Quest|Objective")
	float GetProgress() const;
	virtual float GetProgress_Implementation() const { return 1; }

	UFUNCTION(BlueprintNativeEvent, Category = "Quest|Objective")
	FString GetObjectiveDescription() const;
	virtual FString GetObjectiveDescription_Implementation() const { return GetClass()->GetName(); }
};
