#pragma once

#include "CoreMinimal.h"
#include "AssetDefinitionDefault.h"

namespace QuestEditorCategories
{
    inline TConstArrayView<FAssetCategoryPath> GetQuestCategories()
    {
        static const TArray<FAssetCategoryPath> Categories =
        {
            FAssetCategoryPath(
                NSLOCTEXT("QuestCoreEditor", "CoreCategory", "Core"),
                NSLOCTEXT("QuestCoreEditor", "QuestCategory", "Quest"))
        };
        return Categories;
    }
}