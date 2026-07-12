#include "QuestCoreEditorModule.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Engine/QuestDebug.h"
//#include "AssetTypeAction/AssetTypeActions_QuestDefinition.h"


#define LOCTEXT_NAMESPACE "FQuestCoreEditorModule"

void FQuestCoreEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	// Gives QuestDefinition its own "Quest" section in the right-click
	// create menu instead of falling into Miscellaneous.
	QuestAssetCategory = AssetTools.RegisterAdvancedAssetCategory(
		FName(TEXT("QuestCore")),
		LOCTEXT("QuestCoreAssetCategory", "Quest"));

	// QuestDefinitionAssetTypeActions = MakeShared<FAssetTypeActions_QuestDefinition>(QuestAssetCategory);
	// AssetTools.RegisterAssetTypeActions(QuestDefinitionAssetTypeActions.ToSharedRef());
}

void FQuestCoreEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		// if (QuestDefinitionAssetTypeActions.IsValid())
		// {
		// 	AssetTools.UnregisterAssetTypeActions(QuestDefinitionAssetTypeActions.ToSharedRef());
		// }
	}
	//QuestDefinitionAssetTypeActions.Reset();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FQuestCoreEditorModule, QuestCoreEditor)

DEFINE_LOG_CATEGORY(Log_QuestCore)
