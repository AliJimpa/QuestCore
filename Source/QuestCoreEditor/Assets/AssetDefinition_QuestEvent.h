#pragma once

#include "CoreMinimal.h"
#include "AssetDefinitionDefault.h"
#include "System/QuestEvent.h"
#include "Style/QuestStyles.h"
#include "Category/QuestEditorCategories.h"
#include "AssetDefinition_QuestEvent.generated.h"

#define LOCTEXT_NAMESPACE "AssetDefinition_QuestEvent"

UCLASS()
class QUESTCOREEDITOR_API UAssetDefinition_QuestEvent : public UAssetDefinitionDefault
{
    GENERATED_BODY()

public:
    virtual FText GetAssetDisplayName() const override
    {
        return LOCTEXT("QuestEvent", "Definition");
    }
    virtual FLinearColor GetAssetColor() const override
    {
        // #BA7517 adventure / missionF
        return FLinearColor(FColor(186, 117, 23));
    }
    virtual TSoftClassPtr<UObject> GetAssetClass() const override
    {
        return UQuestEvent::StaticClass();
    }
    virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override
    {
        return QuestEditorCategories::GetQuestCategories();
    }

    virtual const FSlateBrush *GetThumbnailBrush(const FAssetData &InAssetData, const FName InClassName) const override
    {
        return FQuestStyles::Get().GetBrush("ClassThumbnail.QuestEvent");
    }
    virtual const FSlateBrush *GetIconBrush(const FAssetData &InAssetData, const FName InClassName) const override
    {
        return FQuestStyles::Get().GetBrush("ClassIcon.QuestEvent");
    }
};

#undef LOCTEXT_NAMESPACE