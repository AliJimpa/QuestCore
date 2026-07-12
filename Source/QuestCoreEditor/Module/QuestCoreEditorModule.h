#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "AssetTypeCategories.h"

class FQuestCoreEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	EAssetTypeCategories::Type QuestAssetCategory = EAssetTypeCategories::Misc;
	TSharedPtr<class FAssetTypeActions_QuestDefinition> QuestDefinitionAssetTypeActions;
};
