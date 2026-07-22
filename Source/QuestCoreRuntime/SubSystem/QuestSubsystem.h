#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "System/QuestComponent.h"
#include "QuestSubsystem.generated.h"

class UQuestDefinition;

/**
 * Owns the set of known quests for the world and exposes what's
 * available to activate vs currently started. Does not tick - quests
 * push updates to it via NotifyQuestUpdated() whenever they resolve
 * an objective group (see UQuestComponent::UpdateQuest()).
 */
UCLASS()
class QUESTCORERUNTIME_API UQuestSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	bool RegisterQuest(UQuestComponent *Quest);
	void SubmitQuestActivation(UQuestComponent *Quest, bool bIsStarted);
	bool UnregisterQuest(UQuestComponent *Quest);
	void NotifyQuestUpdated(UQuestComponent *Quest);

protected:
	virtual void Initialize(FSubsystemCollectionBase &Collection) override;

private:
	void LoadQuestData();

public:
	UPROPERTY(BlueprintAssignable, Category = "Quest|Events")
	FOnQuestStateChanged OnAnyQuestUpdated;

private:
	UPROPERTY()
	TArray<TObjectPtr<UQuestComponent>> RegisteredQuests;
	UPROPERTY()
	TArray<TObjectPtr<UQuestComponent>> ActiveQuests;
	UPROPERTY()
	TMap<FName, EQuestState> PendingLoadedStates;

public:
	// Writes every currently registered quest's state to disk.
	UFUNCTION(BlueprintCallable, Category = "QuestSubsystem")
	void SaveQuestData();
	/**
	 * Activates the quest matching the given definition asset.
	 * @param WorldContextObject Any object in the target world (used to resolve the QuestSubsystem).
	 * @param Definition The quest's data asset - resolved to a registered UQuestComponent internally.
	 * @return true if a matching quest was found and activation was attempted; false if no quest is registered for this definition.
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestSubsystem")
	bool StartQuest(const UQuestDefinition *Definition);
	/**
	 * Activates the quest matching the given QuestId.
	 * @param WorldContextObject Any object in the target world (used to resolve the QuestSubsystem).
	 * @param QuestId The QuestId to look up (see UQuestDefinition::QuestId).
	 * @return true if a matching quest was found and activation was attempted; false if no quest is registered under this id.
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestSubsystem")
	bool StartQuestById(FName QuestId);
	/**
	 * Deactivates the quest matching the given definition asset.
	 * @param WorldContextObject Any object in the target world (used to resolve the QuestSubsystem).
	 * @param Definition The quest's data asset - resolved to a registered UQuestComponent internally.
	 * @return true if a matching quest was found and deactivation was attempted; false if no quest is registered for this definition.
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestSubsystem")
	bool ResetQuest(const UQuestDefinition *Definition);
	/**
	 * Deactivates the quest matching the given QuestId.
	 * @param WorldContextObject Any object in the target world (used to resolve the QuestSubsystem).
	 * @param QuestId The QuestId to look up (see UQuestDefinition::QuestId).
	 * @return true if a matching quest was found and deactivation was attempted; false if no quest is registered under this id.
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestSubsystem")
	bool ResetQuestById(FName QuestId);

	/**
	 * Returns every quest currently in the Started state.
	 * @return All registered quests whose state is EQuestState::Inprogress.
	 */
	UFUNCTION(BlueprintPure, Category = "QuestSubsystem")
	TArray<UQuestComponent *> GetStartedQuests() const;
	/**
	 * Returns every quest that could be activated right now.
	 * @return All registered quests still NotStarted whose prerequisites are currently satisfied.
	 */
	UFUNCTION(BlueprintPure, Category = "QuestSubsystem")
	TArray<UQuestComponent *> GetAvailableQuests() const;
	/**
	 * Looks up a registered quest by its QuestId.
	 * @param QuestId The QuestId to search for (see UQuestDefinition::QuestId).
	 * @return The matching quest, or nullptr if no registered quest has this id.
	 */
	UFUNCTION(BlueprintPure, Category = "QuestSubsystem")
	UQuestComponent *FindQuestById(FName QuestId) const;
	/**
	 * Looks up a registered quest by its definition asset.
	 * @param Definition The quest definition asset to search for.
	 * @return The matching quest, or nullptr if no registered quest uses this definition.
	 */
	UFUNCTION(BlueprintPure, Category = "QuestSubsystem")
	UQuestComponent *FindQuestByDefinition(const UQuestDefinition *Definition) const;
	/**
	 * Checks whether the quest matching QuestId has completed.
	 * @param WorldContextObject Any object in the target world (used to resolve the QuestSubsystem).
	 * @param QuestId The QuestId to look up (see UQuestDefinition::QuestId).
	 * @return true if a matching quest is registered and its state is Completed; false otherwise (including if not found).
	 */
	UFUNCTION(BlueprintPure, Category = "QuestSubsystem")
	bool IsQuestCompletedById(FName QuestId) const;
	/**
	 * Checks whether the quest matching Definition has completed.
	 * @param WorldContextObject Any object in the target world (used to resolve the QuestSubsystem).
	 * @param Definition The quest definition asset to look up.
	 * @return true if a matching quest is registered and its state is Completed; false otherwise (including if not found).
	 */
	UFUNCTION(BlueprintPure, Category = "QuestSubsystem")
	bool IsQuestCompletedByDefinition(const UQuestDefinition *Definition) const;
	/**
	 * Checks whether the quest matching QuestId has failed.
	 * @param WorldContextObject Any object in the target world (used to resolve the QuestSubsystem).
	 * @param QuestId The QuestId to look up (see UQuestDefinition::QuestId).
	 * @return true if a matching quest is registered and its state is Failed; false otherwise (including if not found).
	 */
	UFUNCTION(BlueprintPure, Category = "QuestSubsystem")
	bool IsQuestFailedById(FName QuestId) const;
	/**
	 * Checks whether the quest matching Definition has failed.
	 * @param WorldContextObject Any object in the target world (used to resolve the QuestSubsystem).
	 * @param Definition The quest definition asset to look up.
	 * @return true if a matching quest is registered and its state is Failed; false otherwise (including if not found).
	 */
	UFUNCTION(BlueprintPure, Category = "QuestSubsystem")
	bool IsQuestFailedByDefinition(const UQuestDefinition *Definition) const;
};
