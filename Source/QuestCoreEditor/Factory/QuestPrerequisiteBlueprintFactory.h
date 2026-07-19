#pragma once

#include "CoreMinimal.h"
#include "Factories/BlueprintFactory.h"
#include "System/QuestPrerequisite.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "QuestPrerequisiteBlueprintFactory.generated.h"

UCLASS()
class QUESTCOREEDITOR_API UQuestPrerequisiteBlueprintFactory : public UBlueprintFactory
{
    GENERATED_BODY()

public:
    UQuestPrerequisiteBlueprintFactory()
    {
        bCreateNew = true;
        bEditAfterNew = true;
        SupportedClass = UBlueprint::StaticClass();
        ParentClass = UQuestPrerequisite::StaticClass();
    }

    virtual FText GetDisplayName() const override
    {
        return NSLOCTEXT("QuestCoreEditor", "QuestObjectiveFactory", "Prerequisite");
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
        ParentClass = UQuestPrerequisite::StaticClass();
        return true;
    }
};