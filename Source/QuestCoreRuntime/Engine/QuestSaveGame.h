#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "System/QuestComponent.h"
#include "QuestSaveGame.generated.h"

/**
 * Save data for the quest system - one EQuestState per QuestId,
 * for every quest that was registered at the time of saving.
 */
UCLASS()
class QUESTCORERUNTIME_API UQuestSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FName, EQuestState> QuestStates;
};