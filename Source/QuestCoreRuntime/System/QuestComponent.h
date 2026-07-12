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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestStateChanged, UQuestComponent *, Quest);

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

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void BeginObjectives();
	void SetState(const EQuestState NewState);
	void EndObjectives();

public:
	UPROPERTY(EditAnywhere, Category = "Quest")
	TObjectPtr<UQuestDefinition> QuestDefinition;
	UPROPERTY(EditAnywhere, Instanced, Category = "Quest")
	TArray<TObjectPtr<UQuestPrerequisite>> Prerequisites;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Quest")
	mutable TArray<TObjectPtr<UQuestObjective>> Objectives;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay, Category = "Quest")
	EQuestState State = EQuestState::NotStarted;

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
	UFUNCTION(BlueprintCallable, Category = "Quest|Functions")
	void ActivateQuest();
	UFUNCTION(BlueprintCallable, Category = "Quest|Functions")
	void DeactivateQuest();

	// Called externally (or by an objective's owner) to re-evaluate the
	// current objective group. Advances order / completes / fails the quest.
	UFUNCTION(BlueprintCallable, Category = "Quest|Functions")
	void UpdateQuest();

	UFUNCTION(BlueprintPure, Category = "Quest|Status")
	bool ArePrerequisitesSatisfied() const;
	UFUNCTION(BlueprintPure, Category = "Quest|Status")
	bool IsQuestActive() const { return State != EQuestState::NotStarted; }
	UFUNCTION(BlueprintPure, Category = "Quest|Status")
	bool IsQuestCompleted() const { return State == EQuestState::Completed; }
	UFUNCTION(BlueprintPure, Category = "Quest|Status")
	bool IsQuestFailed() const { return State == EQuestState::Failed; }

	/**
	 * Returns the currently stored quest state without re-evaluating objectives.
	 * This does not update the quest or trigger any delegates.
	 */
	UFUNCTION(BlueprintPure, Category = "Quest|Getter")
	EQuestState GetQuestState() const { return State; }
	/**
	 * Returns the quest's current state, forcing a re-evaluation first.
	 * NOTE: this has a side effect (UpdateQuest() can advance/complete/fail
	 * the quest and broadcast delegates) despite being marked BlueprintPure -
	 * Blueprint may call pure functions more than once per frame without
	 * your control, which could fire OnQuestUpdated/Completed/Failed more
	 * often than expected. Consider BlueprintCallable instead if that matters.
	 */
	UFUNCTION(BlueprintPure, Category = "Quest|Getter", meta = (ToolTip = "Re-evaluates the quest's current objective group, then returns the resulting quest state (NotStarted / Active / Completed / Failed)."))
	EQuestState EvaluateQuestState()
	{
		UpdateQuest();
		return State;
	}
	UFUNCTION(BlueprintPure, Category = "Quest|Getter")
	float GetProgress() const;
	/**
	 * Returns the first objective still InProgress, in array order.
	 * Since objectives no longer have an explicit Order/step grouping,
	 * this is a simple "first unresolved" lookup - not a "current step"
	 * concept. Returns nullptr if every objective has already resolved
	 * (Done/Failed/Canceled) or the array is empty.
	 */
	UFUNCTION(BlueprintPure, Category = "Quest|Getter")
	UQuestObjective *GetCurrentObjective() const;
	// Convenience passthrough so existing FName-based lookups keep working.
	UFUNCTION(BlueprintPure, Category = "Quest|Getter")
	FName GetQuestId() const { return QuestDefinition ? QuestDefinition->QuestId : NAME_None; }
	UFUNCTION(BlueprintPure, Category = "Quest|Getter")
	UQuestDefinition *GetQuestDefinition() const { return QuestDefinition; }
};
