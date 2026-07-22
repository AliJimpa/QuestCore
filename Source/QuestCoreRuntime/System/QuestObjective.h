#pragma once

#include "CoreMinimal.h"
#include "Engine/QuestDebug.h"
#include "UObject/NoExportTypes.h"
#include "QuestObjective.generated.h"

class UQuestComponent;

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
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, BlueprintType, Blueprintable, CollapseCategories)
class QUESTCORERUNTIME_API UQuestObjective : public UObject
{
	GENERATED_BODY()

public:
	virtual void Construction(UQuestComponent *Quest);

protected:
	virtual class UWorld *GetWorld() const override;

public:
	UPROPERTY(Transient, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	class UWorld *World;

private:
	UPROPERTY()
	UQuestComponent *MyQuest;

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
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Objective|Override")
	EQuestObjectiveState GetState() const;
	virtual EQuestObjectiveState GetState_Implementation() const { return EQuestObjectiveState::InProgress; }

	// Optional 0-1 progress value for UI.
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Objective|Override")
	float GetProgress() const;
	virtual float GetProgress_Implementation() const { return 0.9f; }

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Objective|Override")
	FString GetObjectiveDescription() const;
	virtual FString GetObjectiveDescription_Implementation() const { return GetClass()->GetName(); }

#if WITH_EDITOR
	UFUNCTION(BlueprintNativeEvent, Category = "Quest|Event")
	void OnVisualize(const UQuestComponent *Executer);
	virtual void OnVisualize_Implementation(const UQuestComponent *Executer) {};
#endif

protected:
	UFUNCTION(BlueprintCallable, Category = "Objective|Functions")
	void UpdateQuest();
	UFUNCTION(BlueprintPure, Category = "Objective|Getter")
	UQuestComponent *GetQuestComponent() const;
	UFUNCTION(BlueprintPure, Category = "Objective|Getter")
	AActor *GetOwner() const;
};
