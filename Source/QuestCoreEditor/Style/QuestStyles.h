#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "Brushes/SlateImageBrush.h"

class FQuestStyles : public FSlateStyleSet
{
public:
    static FQuestStyles &Get()
    {
        static FQuestStyles Instance;
        return Instance;
    }

private:
    FQuestStyles() : FSlateStyleSet("QuestStyles")
    {
        const FString PluginResourcesDir = IPluginManager::Get().FindPlugin(TEXT("QuestCore"))->GetBaseDir() / TEXT("Resources");
        SetContentRoot(PluginResourcesDir);

        // Definition
        Set("ClassIcon.QuestDefinition", new FSlateImageBrush(
                                             RootToContentDir(TEXT("QuestDefinition"), TEXT(".png")),
                                             FVector2D(16.f, 16.f),
                                             FLinearColor::White));

        Set("ClassThumbnail.QuestDefinition", new FSlateImageBrush(
                                                  RootToContentDir(TEXT("QuestDefinition"), TEXT(".png")),
                                                  FVector2D(64.f, 64.f),
                                                  FLinearColor::White));

        // Objective
        Set("ClassIcon.QuestObjective", new FSlateImageBrush(
                                            RootToContentDir(TEXT("QuestObjective"), TEXT(".png")),
                                            FVector2D(16.f, 16.f),
                                            FLinearColor::White));

        Set("ClassThumbnail.QuestObjective", new FSlateImageBrush(
                                                 RootToContentDir(TEXT("QuestObjective"), TEXT(".png")),
                                                 FVector2D(64.f, 64.f),
                                                 FLinearColor::White));

        FSlateStyleRegistry::RegisterSlateStyle(*this);
    }

    ~FQuestStyles()
    {
        FSlateStyleRegistry::UnRegisterSlateStyle(*this);
    }
};