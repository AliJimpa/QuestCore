#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestObjective.h"
#include "QuestPrerequisite.h"
#include "QuestDefinition.h"
#include "QuestComponent.generated.h"

UENUM(BlueprintType)
enum class EQuestState : uint8
{
	NotStarted,
	Active,
	Completed,
	Failed
};

USTRUCT(BlueprintType)
struct FQuestObjectiveEntry
{
	GENERATED_BODY()

	// Objectives that share the same Order are checked as a group -
	// all of them must resolve Done before the quest advances.
	UPROPERTY(EditAnywhere, Instanced, Category = "Quest")
	TObjectPtr<UQuestObjective> Objective = nullptr;

	UPROPERTY(EditAnywhere, Category = "Quest")
	int32 Order = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestStateChanged, UQuestComponent*, Quest);

/**
 * A Quest. Holds an ordered list of objective groups and evaluates
 * them only when UpdateQuest() is called - no ticking. Objectives
 * within the same Order all run in parallel; the quest advances to
 * the next Order once every objective in the current group is Done.
 */
UCLASS(ClassGroup = (Quest), meta = (BlueprintSpawnableComponent))
class QUESTCORERUNTIME_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Quest")
	TObjectPtr<UQuestDefinition> QuestDefinition;

	// Convenience passthrough so existing FName-based lookups keep working.
	UFUNCTION(BlueprintPure, Category = "Quest")
	FName GetQuestId() const { return QuestDefinition ? QuestDefinition->QuestId : NAME_None; }

	UPROPERTY(EditAnywhere, Category = "Quest")
	TArray<FQuestObjectiveEntry> Objectives;

	UPROPERTY(EditAnywhere, Instanced, Category = "Quest")
	TArray<TObjectPtr<UQuestPrerequisite>> Prerequisites;

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestStateChanged OnQuestCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestStateChanged OnQuestFailed;

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestStateChanged OnQuestUpdated;

	UFUNCTION(BlueprintPure, Category = "Quest")
	EQuestState GetQuestState() const { return State; }

	UFUNCTION(BlueprintPure, Category = "Quest")
	int32 GetCurrentOrder() const { return CurrentOrder; }

	UFUNCTION(BlueprintPure, Category = "Quest")
	bool ArePrerequisitesSatisfied() const;

	// Starts the quest: moves to Active, begins the first objective group.
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void ActivateQuest();

	// Called externally (or by an objective's owner) to re-evaluate the
	// current objective group. Advances order / completes / fails the quest.
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void UpdateQuest();

	// Returns the overall 0-1 progress of the current objective group.
	UFUNCTION(BlueprintPure, Category = "Quest")
	float GetCurrentGroupProgress() const;

private:
	TArray<UQuestObjective*> GetCurrentGroupObjectives() const;
	void BeginCurrentGroup();
	void EndCurrentGroup();
	void AdvanceOrFinish();

	UPROPERTY(VisibleInstanceOnly, Category = "Quest")
	EQuestState State = EQuestState::NotStarted;

	UPROPERTY(VisibleInstanceOnly, Category = "Quest")
	int32 CurrentOrder = 0;
};
