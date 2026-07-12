#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "QuestSubsystem.generated.h"

class UQuestComponent;
class UQuestDefinition;

/**
 * Owns the set of known quests for the world and exposes what's
 * available to activate vs currently active. Does not tick - quests
 * push updates to it via NotifyQuestUpdated() whenever they resolve
 * an objective group (see UQuestComponent::UpdateQuest()).
 */
UCLASS()
class QUESTCORERUNTIME_API UQuestSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	// Called by UQuestComponent whenever it changes state (activated, updated, completed, failed).
	void NotifyQuestUpdated(UQuestComponent *Quest);

private:
	UPROPERTY()
	TArray<TObjectPtr<UQuestComponent>> RegisteredQuests;

	UPROPERTY()
	TArray<TObjectPtr<UQuestComponent>> ActiveQuests;

public:
	// Called by a QuestComponent (e.g. BeginPlay) to make itself known to the system.
	UFUNCTION(BlueprintCallable, Category = "Quest|Functions")
	void RegisterQuest(UQuestComponent *Quest);

	UFUNCTION(BlueprintCallable, Category = "Quest|Functions")
	void UnregisterQuest(UQuestComponent *Quest);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool ActivateQuest(UQuestDefinition *Definition);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool ActivateQuestById(FName QuestId);

	UFUNCTION(BlueprintCallable, Category = "Quest|Getter")
	TArray<UQuestComponent *> GetActiveQuests() const;

	// Quests whose prerequisites are satisfied but that aren't active/completed yet.
	UFUNCTION(BlueprintCallable, Category = "Quest|Getter")
	TArray<UQuestComponent *> GetAvailableQuests() const;

	UFUNCTION(BlueprintCallable, Category = "Quest|Getter")
	UQuestComponent *FindQuestById(FName QuestId) const;

	UFUNCTION(BlueprintCallable, Category = "Quest|Getter")
	UQuestComponent *FindQuestByDefinition(UQuestDefinition *Definition) const;
};
