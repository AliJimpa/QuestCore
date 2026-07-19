#include "QuestCoreEditorModule.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"

#include "ToolMenus.h"


#include "Assets\AssetTypeActions_QuestObjective.h"

#define LOCTEXT_NAMESPACE "FQuestCoreEditorModule"

void FQuestCoreEditorModule::StartupModule()
{
    FAssetToolsModule &AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
    IAssetTools &AssetTools = AssetToolsModule.Get();
    EAssetTypeCategories::Type CoreCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Core")), NSLOCTEXT("QuestCoreEditor", "QuestAssetCategory", "Quest"));
    TSharedRef<FAssetTypeActions_QuestObjective> QuestObjectiveActions = MakeShared<FAssetTypeActions_QuestObjective>(CoreCategoryBit);
    AssetTools.RegisterAssetTypeActions(QuestObjectiveActions);
    RegisteredAssetTypeActions.Add(QuestObjectiveActions);
}

void FQuestCoreEditorModule::ShutdownModule()
{
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        FAssetToolsModule &AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
        for (auto &Action : RegisteredAssetTypeActions)
        {
            AssetToolsModule.Get().UnregisterAssetTypeActions(Action.ToSharedRef());
        }
    }
    RegisteredAssetTypeActions.Empty();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FQuestCoreEditorModule, QuestCoreEditor)