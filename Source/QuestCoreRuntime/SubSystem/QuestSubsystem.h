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
	void RegisterQuest(UQuestComponent *Quest);
	void SubmitQuestActivation(UQuestComponent *Quest, bool bIsActive);
	void UnregisterQuest(UQuestComponent *Quest);

private:
	UPROPERTY()
	TArray<TObjectPtr<UQuestComponent>> RegisteredQuests;
	UPROPERTY()
	TArray<TObjectPtr<UQuestComponent>> ActiveQuests;

public:
	UFUNCTION(BlueprintCallable, Category = "Quest|Functions")
	bool ActivateQuest(UQuestDefinition *Definition);
	UFUNCTION(BlueprintCallable, Category = "Quest|Functions")
	bool ActivateQuestById(FName QuestId);
	UFUNCTION(BlueprintCallable, Category = "Quest|Functions")
	bool DeactivateQuest(UQuestDefinition *Definition);
	UFUNCTION(BlueprintCallable, Category = "Quest|Functions")
	bool DeactivateQuestById(FName QuestId);

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
