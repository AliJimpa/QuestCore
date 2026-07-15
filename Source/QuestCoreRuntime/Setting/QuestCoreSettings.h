#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/QuestSaveGame.h"
#include "QuestCoreSettings.generated.h"

/**
 * Project-wide settings for QuestCore. Shows up under
 * Project Settings -> Game -> Quest Core. Backed by Config, so values
 * persist to DefaultGame.ini rather than living per-subsystem-instance.
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

    UPROPERTY(Config, EditAnywhere, Category = "Save")
    FString SaveSlotName = TEXT("QuestSaveSlot");

    UPROPERTY(Config, EditAnywhere, Category = "Save")
    int32 SaveUserIndex = 0;

    // Which USaveGame class the subsystem creates/loads. Point this at
    // a project-specific subclass of UQuestSaveGame to persist extra
    // data alongside QuestStates without touching QuestSubsystem itself.
    UPROPERTY(Config, EditAnywhere, Category = "Save")
    TSubclassOf<UQuestSaveGame> SaveGameClass = UQuestSaveGame::StaticClass();
};