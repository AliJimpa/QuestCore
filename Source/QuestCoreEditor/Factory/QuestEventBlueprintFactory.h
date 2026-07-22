#pragma once

#include "CoreMinimal.h"
#include "Factories/BlueprintFactory.h"
#include "System/QuestEvent.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "QuestEventBlueprintFactory.generated.h"

UCLASS()
class QUESTCOREEDITOR_API UQuestEventBlueprintFactory : public UBlueprintFactory
{
    GENERATED_BODY()

public:
    UQuestEventBlueprintFactory()
    {
        bCreateNew = true;
        bEditAfterNew = true;
        SupportedClass = UBlueprint::StaticClass();
        ParentClass = UQuestEvent::StaticClass();
    }

    virtual FText GetDisplayName() const override
    {
        return NSLOCTEXT("QuestCoreEditor", "QuestObjectiveFactory", "QuestEvent");
    }

    virtual uint32 GetMenuCategories() const override
    {
        FAssetToolsModule &AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
        return AssetToolsModule.Get().FindAdvancedAssetCategory(FName(TEXT("Core")));
    }

    virtual const TArray<FText> &GetMenuCategorySubMenus() const override
    {
        static const TArray<FText> SubMenus{NSLOCTEXT("QuestCoreEditor", "QuestSubMenu", "Quest")};
        return SubMenus;
    }

    virtual bool ConfigureProperties() override
    {
        ParentClass = UQuestEvent::StaticClass();
        return true;
    }
};