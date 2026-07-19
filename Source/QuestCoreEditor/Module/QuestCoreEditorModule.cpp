#include "QuestCoreEditorModule.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FQuestCoreEditorModule"

void FQuestCoreEditorModule::StartupModule()
{
    FAssetToolsModule &AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
    IAssetTools &AssetTools = AssetToolsModule.Get();

    AssetTools.RegisterAdvancedAssetCategory(
        FName(TEXT("Core")), NSLOCTEXT("QuestCoreEditor", "CoreAssetCategory", "Core"));
}

void FQuestCoreEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FQuestCoreEditorModule, QuestCoreEditor)