#pragma once

#include "CoreMinimal.h"
#include "AssetDefinitionDefault.h"
#include "System/QuestPrerequisite.h"
#include "Style/QuestStyles.h"
#include "Category/QuestEditorCategories.h"
#include "AssetDefinition_QuestPrerequisite.generated.h"

#define LOCTEXT_NAMESPACE "AssetDefinition_QuestPrerequisite"

UCLASS()
class QUESTCOREEDITOR_API UAssetDefinition_QuestPrerequisite : public UAssetDefinitionDefault
{
    GENERATED_BODY()

public:
    virtual FText GetAssetDisplayName() const override
    {
        return LOCTEXT("QuestPrerequisite", "Definition");
    }
    virtual FLinearColor GetAssetColor() const override
    {
        // #BA7517 adventure / missionF
        return FLinearColor(FColor(186, 117, 23));
    }
    virtual TSoftClassPtr<UObject> GetAssetClass() const override
    {
        return UQuestPrerequisite::StaticClass();
    }
    virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override
    {
        return QuestEditorCategories::GetQuestCategories();
    }

    virtual const FSlateBrush *GetThumbnailBrush(const FAssetData &InAssetData, const FName InClassName) const override
    {
        return FQuestStyles::Get().GetBrush("ClassThumbnail.QuestPrerequisite");
    }
    virtual const FSlateBrush *GetIconBrush(const FAssetData &InAssetData, const FName InClassName) const override
    {
        return FQuestStyles::Get().GetBrush("ClassIcon.QuestPrerequisite");
    }
};

#undef LOCTEXT_NAMESPACE