#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/QuestSaveGame.h"
#include "QuestCoreSettings.generated.h"

/**
 * Configure the CoreQuest Plugin
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Quest Core"))
class QUESTCORERUNTIME_API UQuestCoreSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UQuestCoreSettings()
    {
        CategoryName = TEXT("Plugins");
    }

public:
    // Which USaveGame class the subsystem creates/loads. Point this at
    // a project-specific subclass of UQuestSaveGame to persist extra
    // data alongside QuestStates without touching QuestSubsystem itself.
    UPROPERTY(Config, EditAnywhere, Category = "Save")
    TSubclassOf<UQuestSaveGame> SaveGameClass = UQuestSaveGame::StaticClass();

    UPROPERTY(Config, EditAnywhere, Category = "Save")
    FString SaveSlotName = TEXT("QuestSaveSlot");

    UPROPERTY(Config, EditAnywhere, Category = "Save")
    int32 SaveUserIndex = 0;
};