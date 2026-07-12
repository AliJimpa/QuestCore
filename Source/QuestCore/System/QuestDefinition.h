#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestDefinition.generated.h"

/**
 * Data asset identity for a quest. Referenced by UQuestComponent
 * instead of a bare FName, so quests are actual browsable/reference-
 * able assets (and can carry designer-facing data like display name
 * and description without touching code).
 */
UCLASS(BlueprintType)
class QUESTCORE_API UQuestDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	// Unique gameplay id used for lookups (QuestSubsystem::ActivateQuest, etc).
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FName QuestId;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, Category = "Quest", meta = (MultiLine = true))
	FText Description;

#if WITH_EDITOR
	// Convenience: default QuestId to the asset name so designers don't
	// have to fill it in manually right after creating the asset.
	virtual void PostInitProperties() override
	{
		Super::PostInitProperties();
		if (QuestId.IsNone())
		{
			QuestId = GetFName();
		}
	}
#endif
};
