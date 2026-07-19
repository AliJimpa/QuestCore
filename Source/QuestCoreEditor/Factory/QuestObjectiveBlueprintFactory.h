#pragma once

#include "CoreMinimal.h"
#include "Factories/BlueprintFactory.h"
#include "System/QuestObjective.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "QuestObjectiveBlueprintFactory.generated.h"

UCLASS()
class QUESTCOREEDITOR_API UQuestObjectiveBlueprintFactory : public UBlueprintFactory
{
    GENERATED_BODY()

public:
    UQuestObjectiveBlueprintFactory()
    {
        bCreateNew = true;
        bEditAfterNew = true;
        SupportedClass = UBlueprint::StaticClass();
        ParentClass = UQuestObjective::StaticClass();
    }

    virtual FText GetDisplayName() const override
    {
        return NSLOCTEXT("QuestCoreEditor", "QuestObjectiveFactory", "Objective");
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
        ParentClass = UQuestObjective::StaticClass();
        return true;
    }
};