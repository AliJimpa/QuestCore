#include "AssetDefinition_QuestDefinition.h"
#include "System/QuestDefinition.h"
#include "Style/QuestDefinitionStyle.h"

#define LOCTEXT_NAMESPACE "QuestAssetDefinition"

FText UAssetDefinition_QuestDefinition::GetAssetDisplayName() const
{
    return LOCTEXT("QuestDefinition", "Definition");
}

FLinearColor UAssetDefinition_QuestDefinition::GetAssetColor() const
{
    return QUESTDEFINITION_COLOR;
}

TSoftClassPtr<UObject> UAssetDefinition_QuestDefinition::GetAssetClass() const
{
    return UQuestDefinition::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_QuestDefinition::GetAssetCategories() const
{
    static const FAssetCategoryPath Categories[] =
        {
            FAssetCategoryPath(
                LOCTEXT("Core", "Core"),
                LOCTEXT("Quest", "Quest"))};

    return Categories;
}

const FSlateBrush *UAssetDefinition_QuestDefinition::GetThumbnailBrush(const FAssetData &InAssetData, const FName InClassName) const
{
    return FQuestDefinitionStyle::Get().GetBrush("ClassThumbnail.QuestDefinition");
}

const FSlateBrush *UAssetDefinition_QuestDefinition::GetIconBrush(const FAssetData &InAssetData, const FName InClassName) const
{
    return FQuestDefinitionStyle::Get().GetBrush("ClassIcon.QuestDefinition");
}

#undef LOCTEXT_NAMESPACE