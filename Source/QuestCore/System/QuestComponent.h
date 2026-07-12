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

	UPROPERTY(EditAnywhere, Category = "ObjectiveEntry")
	int32 Order = 0;

	// Objectives that share the same Order are checked as a group -
	// all of them must resolve Done before the quest advances.
	UPROPERTY(EditAnywhere, Instanced, Category = "ObjectiveEntry")
	TObjectPtr<UQuestObjective> Objective = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestStateChanged, UQuestComponent *, Quest);

/**
 * A Quest. Holds an ordered list of objective groups and evaluates
 * them only when UpdateQuest() is called - no ticking. Objectives
 * within the same Order all run in parallel; the quest advances to
 * the next Order once every objective in the current group is Done.
 */
UCLASS(ClassGroup = (Quest), meta = (BlueprintSpawnableComponent))
class QUESTCORE_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void BeginCurrentGroup();
	void EndCurrentGroup();
	void AdvanceOrFinish();

public:
	UPROPERTY(EditAnywhere, Category = "Quest")
	TObjectPtr<UQuestDefinition> QuestDefinition;

	UPROPERTY(EditAnywhere, Instanced, Category = "Quest")
	TArray<TObjectPtr<UQuestPrerequisite>> Prerequisites;

	UPROPERTY(EditAnywhere, Category = "Quest")
	TArray<FQuestObjectiveEntry> Objectives;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay, Category = "Quest")
	EQuestState State = EQuestState::NotStarted;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay, Category = "Quest")
	int32 CurrentOrder = 0;

	UPROPERTY(BlueprintAssignable, Category = "Quest|Events")
	FOnQuestStateChanged OnQuestCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Quest|Events")
	FOnQuestStateChanged OnQuestFailed;

	UPROPERTY(BlueprintAssignable, Category = "Quest|Events")
	FOnQuestStateChanged OnQuestUpdated;

protected:
	UPROPERTY(EditAnywhere, Category = "Quest|Setting")
	bool bAutoActive = false;

public:
	// Starts the quest: moves to Active, begins the first objective group.
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void ActivateQuest();

	// Called externally (or by an objective's owner) to re-evaluate the
	// current objective group. Advances order / completes / fails the quest.
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void UpdateQuest();

	UFUNCTION(BlueprintPure, Category = "Quest|Getter")
	TArray<UQuestObjective *> GetGroupObjectives(int32 Order) const;

	// Convenience passthrough so existing FName-based lookups keep working.
	UFUNCTION(BlueprintPure, Category = "Quest")
	FName GetQuestId() const { return QuestDefinition ? QuestDefinition->QuestId : NAME_None; }

	UFUNCTION(BlueprintPure, Category = "Quest")
	EQuestState GetQuestState() const { return State; }

	UFUNCTION(BlueprintPure, Category = "Quest")
	int32 GetCurrentOrder() const { return CurrentOrder; }

	UFUNCTION(BlueprintPure, Category = "Quest")
	bool ArePrerequisitesSatisfied() const;

	// Returns the overall 0-1 progress of the current objective group.
	UFUNCTION(BlueprintPure, Category = "Quest")
	float GetCurrentGroupProgress() const;
};
