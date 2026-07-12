#include "AssetTypeActions_QuestDefinition.h"
#include "System/QuestDefinition.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions_QuestDefinition"

FAssetTypeActions_QuestDefinition::FAssetTypeActions_QuestDefinition(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FAssetTypeActions_QuestDefinition::GetName() const
{
	return LOCTEXT("AssetTypeActions_QuestDefinition_Name", "Quest Definition");
}

FColor FAssetTypeActions_QuestDefinition::GetTypeColor() const
{
	return FColor(80, 200, 120);
}

UClass* FAssetTypeActions_QuestDefinition::GetSupportedClass() const
{
	return UQuestDefinition::StaticClass();
}

uint32 FAssetTypeActions_QuestDefinition::GetCategories()
{
	return AssetCategory;
}

#undef LOCTEXT_NAMESPACE
