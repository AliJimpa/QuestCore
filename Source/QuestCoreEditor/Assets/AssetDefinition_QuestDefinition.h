// AssetDefinition_QuestDefinition.h

#pragma once

#include "AssetDefinitionDefault.h"
#include "AssetDefinition_QuestDefinition.generated.h"

UCLASS()
class QUESTCOREEDITOR_API UAssetDefinition_QuestDefinition : public UAssetDefinitionDefault
{
    GENERATED_BODY()

public:

    virtual FText GetAssetDisplayName() const override;
    virtual FLinearColor GetAssetColor() const override;
    virtual TSoftClassPtr<UObject> GetAssetClass() const override;
    virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
};