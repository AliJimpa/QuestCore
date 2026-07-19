#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "Style/QuestStyles.h"
#include "System/QuestPrerequisite.h"

class FAssetTypeActions_QuestPrerequisite : public FAssetTypeActions_Base
{
public:
    FAssetTypeActions_QuestPrerequisite(EAssetTypeCategories::Type InAssetCategoryBit)
        : AssetCategoryBit(InAssetCategoryBit)
    {
    }

    virtual FText GetName() const override
    {
        return NSLOCTEXT("QuestCoreEditor", "AssetTypeName_QuestPrerequisite", "Prerequisite");
    }

    virtual FColor GetTypeColor() const override
    {
        return FColor(186, 117, 23);
    }

    virtual UClass *GetSupportedClass() const override
    {
        return UQuestPrerequisite::StaticClass();
    }

    virtual uint32 GetCategories() override
    {
        return AssetCategoryBit;
    }

    // This is what nests the entry under "Quest" inside your Core category
    virtual const TArray<FText> &GetSubMenus() const override
    {
        static const TArray<FText> SubMenus{NSLOCTEXT("QuestCoreEditor", "QuestSubMenu", "Quest")};
        return SubMenus;
    }

    virtual const FSlateBrush *GetThumbnailBrush(const FAssetData &InAssetData, const FName InClassName) const override
    {
        return FQuestStyles::Get().GetBrush("ClassThumbnail.QuestPrerequisite");
    }

    virtual const FSlateBrush *GetIconBrush(const FAssetData &InAssetData, const FName InClassName) const override
    {
        return FQuestStyles::Get().GetBrush("ClassIcon.QuestPrerequisite");
    }

private:
    EAssetTypeCategories::Type AssetCategoryBit;
};