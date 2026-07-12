#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class FAssetTypeActions_QuestDefinition : public FAssetTypeActions_Base
{
public:
	explicit FAssetTypeActions_QuestDefinition(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type AssetCategory;
};
