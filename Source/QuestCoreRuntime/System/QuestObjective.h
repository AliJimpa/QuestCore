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

private:
	UPROPERTY()
	TWeakObjectPtr<AActor> MyOwner;
	UPROPERTY()
	TWeakObjectPtr<UQuestDefinition> MyDefination;

public:
	UFUNCTION()
	virtual void Construction(AActor *Owner, UQuestDefinition *Defination)
	{
		MyOwner = Owner;
		MyDefination = Defination;
	}

protected:
	UFUNCTION(BlueprintPure, Category = "Objective|Getter")
	AActor *GetOwner() const { return MyOwner.Get(); }
	UFUNCTION(BlueprintPure, Category = "Objective|Getter")
	UQuestDefinition *GetDefination() const { return MyDefination.Get(); }

public:
	// Called by the owning Quest when this objective becomes active
	UFUNCTION(BlueprintNativeEvent, Category = "Objective|Event")
	void Begin();
	virtual void Begin_Implementation() {}

	// Called by the owning Quest when this objective is done resolving
	// (Done or Failed). Always called - use it for cleanup (unbinding etc).
	UFUNCTION(BlueprintNativeEvent, Category = "Objective|Event")
	void End();
	virtual void End_Implementation() {}

	// Single source of truth the Quest reads on UpdateQuest().
	UFUNCTION(BlueprintNativeEvent, Category = "Objective|Override")
	EQuestObjectiveState GetState() const;
	virtual EQuestObjectiveState GetState_Implementation() const { return EQuestObjectiveState::InProgress; }

	// Optional 0-1 progress value for UI.
	UFUNCTION(BlueprintNativeEvent, Category = "Objective|Override")
	float GetProgress() const;
	virtual float GetProgress_Implementation() const { return 1; }

	UFUNCTION(BlueprintNativeEvent, Category = "Objective|Override")
	FString GetObjectiveDescription() const;
	virtual FString GetObjectiveDescription_Implementation() const { return GetClass()->GetName(); }
};
