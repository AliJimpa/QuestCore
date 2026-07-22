#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestComponent.generated.h"

class UQuestDefinition;
class UQuestObjective;
class UQuestSubsystem;
class UQuestEvent;

UENUM(BlueprintType)
enum class EQuestState : uint8
{
	NotStarted,
	InProgress,
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
UCLASS(Blueprintable, BlueprintType, ClassGroup = (QuestCore), meta = (BlueprintSpawnableComponent, DisplayName = "QuestComponent"))
class QUESTCORERUNTIME_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void ApplyLoadedState(EQuestState SavedState);
	UQuestSubsystem *GetQuestSubsystem() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void BeginObjectives();
	void SetState(const EQuestState NewState);
	void InvokeQuestEvents(bool IsCompleted);
	void EndObjectives();
#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Quest")
	void Visualize();
#endif

private:
	mutable TWeakObjectPtr<UQuestSubsystem> CachedSubsystem;

public:
	UPROPERTY(BlueprintAssignable, Category = "Quest|Events")
	FOnQuestStateChanged OnQuestCompleted;
	UPROPERTY(BlueprintAssignable, Category = "Quest|Events")
	FOnQuestStateChanged OnQuestFailed;
	UPROPERTY(BlueprintAssignable, Category = "Quest|Events")
	FOnQuestStateChanged OnQuestUpdated;

protected:
	UPROPERTY(EditAnywhere, Category = "Quest")
	UQuestDefinition *QuestDefinition;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	bool bAutoActive = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest", meta = (Tooltip = "If true after the quest Completed or Failed automaticly destroed actor"))
	bool bAutoDestroy = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	bool bAutoSave = false;

private:
	UPROPERTY(VisibleAnywhere, AdvancedDisplay, Category = "Quest")
	EQuestState State = EQuestState::NotStarted;

public:
	// Starts the quest: moves to Start, begins the first objective group.
	UFUNCTION(BlueprintCallable, Category = "Quest|Functions")
	bool StartQuest();
	UFUNCTION(BlueprintCallable, Category = "Quest|Functions")
	bool ResetQuest();
	/**
	 * Called externally (or by an objective's owner) to re-evaluate the
	 * current objective group. Advances order / completes / fails the quest.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest|Functions")
	void UpdateQuest();
	/**
	 * Returns the quest's current state, forcing a re-evaluation first.
	 * NOTE: this has a side effect (UpdateQuest() can advance/complete/fail
	 * the quest and broadcast delegates) despite being marked BlueprintPure -
	 * Blueprint may call pure functions more than once per frame without
	 * your control, which could fire OnQuestUpdated/Completed/Failed more
	 * often than expected. Consider BlueprintCallable instead if that matters.
	 */
	UFUNCTION(BlueprintPure, Category = "Quest|Getter", meta = (ToolTip = "Re-evaluates the quest's current objective group, then returns the resulting quest state (NotStarted / Inprogress / Completed / Failed)."))
	const EQuestState EvaluateQuestState()
	{
		UpdateQuest();
		return State;
	}

	UFUNCTION(BlueprintPure, Category = "Quest|Status")
	bool ArePrerequisitesSatisfied() const;
	UFUNCTION(BlueprintPure, Category = "Quest|Status")
	bool IsQuestStarted() const { return State != EQuestState::NotStarted; }
	UFUNCTION(BlueprintPure, Category = "Quest|Status")
	bool IsQuestCompleted() const { return State == EQuestState::Completed; }
	UFUNCTION(BlueprintPure, Category = "Quest|Status")
	bool IsQuestFailed() const { return State == EQuestState::Failed; }
	UFUNCTION(BlueprintPure, Category = "Quest|Status")
	bool IsQuestEnded() const { return State != EQuestState::NotStarted && State != EQuestState::InProgress; }

	/**
	 * Returns the currently stored quest state without re-evaluating objectives.
	 * This does not update the quest or trigger any delegates.
	 */
	UFUNCTION(BlueprintPure, Category = "Quest|Getter")
	const EQuestState GetQuestState() const { return State; }
	UFUNCTION(BlueprintPure, Category = "Quest|Getter")
	UQuestDefinition *GetDefinition() const { return QuestDefinition; }
	// Convenience passthrough so existing FName-based lookups keep working.
	UFUNCTION(BlueprintPure, Category = "Quest|Getter")
	FName GetQuestId() const;
	UFUNCTION(BlueprintPure, Category = "Quest|Getter")
	float GetProgress() const;
	UFUNCTION(BlueprintPure, Category = "Quest|Getter")
	int32 GetMaxObjective() const;
	UFUNCTION(BlueprintPure, Category = "Quest|Getter")
	int32 GetCurrentObjectiveIndex() const;
	/**
	 * Returns the first objective still InProgress, in array order.
	 * Since objectives no longer have an explicit Order/step grouping,
	 * this is a simple "first unresolved" lookup - not a "current step"
	 * concept. Returns nullptr if every objective has already resolved
	 * (Done/Failed/Canceled) or the array is empty.
	 */
	UFUNCTION(BlueprintPure, Category = "Quest|Getter")
	UQuestObjective *GetObjective(const int32 Index) const;

protected:
};
