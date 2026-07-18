#include "AssetDefinition_QuestDefinition.h"
#include "System/QuestDefinition.h"

#define LOCTEXT_NAMESPACE "QuestAssetDefinition"

FText UAssetDefinition_QuestDefinition::GetAssetDisplayName() const
{
    return LOCTEXT("QuestDefinition", "Definition");
}

FLinearColor UAssetDefinition_QuestDefinition::GetAssetColor() const
{
    // #BA7517 adventure / mission
    return FLinearColor(FColor(186, 117, 23));
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

#undef LOCTEXT_NAMESPACE