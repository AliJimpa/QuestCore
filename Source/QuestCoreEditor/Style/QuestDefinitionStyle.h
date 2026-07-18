#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "Brushes/SlateImageBrush.h"

// #BA7517 adventure / mission
#define QUESTDEFINITION_COLOR FLinearColor(FColor(186, 117, 23))

class FQuestDefinitionStyle : public FSlateStyleSet
{
public:
    static FQuestDefinitionStyle &Get()
    {
        static FQuestDefinitionStyle Instance;
        return Instance;
    }

private:
    FQuestDefinitionStyle() : FSlateStyleSet("QuestDefinitionStyle")
    {
        const FString PluginResourcesDir = IPluginManager::Get().FindPlugin(TEXT("QuestCore"))->GetBaseDir() / TEXT("Resources");
        SetContentRoot(PluginResourcesDir);

        // Small icon used in menus, right-click context menu, toolbars, etc.
        Set("ClassIcon.QuestDefinition", new FSlateImageBrush(
                                             RootToContentDir(TEXT("QuestDefinition"), TEXT(".png")),
                                             FVector2D(16.f, 16.f),
                                             FLinearColor::White));

        // Larger thumbnail used in Content Browser tiles
        Set("ClassThumbnail.QuestDefinition", new FSlateImageBrush(
                                                  RootToContentDir(TEXT("QuestDefinition"), TEXT(".png")),
                                                  FVector2D(64.f, 64.f),
                                                  FLinearColor::White));

        FSlateStyleRegistry::RegisterSlateStyle(*this);
    }

    ~FQuestDefinitionStyle()
    {
        FSlateStyleRegistry::UnRegisterSlateStyle(*this);
    }
};