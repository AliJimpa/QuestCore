#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "System/QuestComponent.h"
#include "QuestSubsystem.generated.h"

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
	bool RegisterQuest(UQuestComponent *Quest);
	void SubmitQuestActivation(UQuestComponent *Quest, bool bIsActive);
	bool UnregisterQuest(UQuestComponent *Quest);
	TArray<UQuestComponent *> GetActiveQuests() const;
	TArray<UQuestComponent *> GetAvailableQuests() const;
	UQuestComponent *FindQuestById(FName QuestId) const;
	UQuestComponent *FindQuestByDefinition(UQuestDefinition *Definition) const;
	bool IsQuestCompletedById(FName QuestId) const;
	bool IsQuestCompletedByDefinition(UQuestDefinition *Definition) const;
	bool IsQuestFailedById(FName QuestId) const;
	bool IsQuestFailedByDefinition(UQuestDefinition *Definition) const;

private:
	UPROPERTY()
	TArray<TObjectPtr<UQuestComponent>> RegisteredQuests;
	UPROPERTY()
	TArray<TObjectPtr<UQuestComponent>> ActiveQuests;

public:
	UPROPERTY(BlueprintAssignable, Category = "Quest|Events")
	FOnQuestStateChanged OnQuestStateChanged;
};
