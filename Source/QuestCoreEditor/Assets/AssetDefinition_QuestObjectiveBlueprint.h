#pragma once

#include "CoreMinimal.h"
#include "AssetDefinitionDefault.h"
#include "System/QuestObjective.h"
#include "Style/QuestStyles.h"
#include "Category/QuestEditorCategories.h"
#include "AssetDefinition_QuestObjectiveBlueprint.generated.h"

#define LOCTEXT_NAMESPACE "UAssetDefinition_QuestObjectiveBlueprint"

UCLASS()
class QUESTCOREEDITOR_API UAssetDefinition_QuestObjectiveBlueprint : public UAssetDefinitionDefault
{
    GENERATED_BODY()

public:
    virtual FText GetAssetDisplayName() const override
    {
        // return NSLOCTEXT("QuestCoreEditor", "AssetTypeName_QuestObjective", "Quest Objective");
        return LOCTEXT("QuestObjective", "Objective");
    }

    virtual FLinearColor GetAssetColor() const override
    {
        // #BA7517 adventure / missionF
        return FLinearColor(FColor(186, 117, 23));
        //return QuestIconColor(186, 117, 23);
    }

    virtual TSoftClassPtr<UObject> GetAssetClass() const override
    {
        return UQuestObjective::StaticClass();
    }

    virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override
    {
        return QuestEditorCategories::GetQuestCategories();
    }

    virtual const FSlateBrush *GetThumbnailBrush(const FAssetData &InAssetData, const FName InClassName) const override
    {
        return FQuestStyles::Get().GetBrush("ClassThumbnail.QuestObjective");
    }
    virtual const FSlateBrush *GetIconBrush(const FAssetData &InAssetData, const FName InClassName) const override
    {
        return FQuestStyles::Get().GetBrush("ClassIcon.QuestObjective");
    }
};

#undef LOCTEXT_NAMESPACE