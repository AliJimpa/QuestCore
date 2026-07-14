#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestDefinition.generated.h"

class UQuestObjective;
class UQuestPrerequisite;

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	Main,
	Side,
	Daily,
	Weekly,
	Tutorial,
	Event,
	Hidden,
	Repeatable,
	Achievement,
	Special,
	Custom
};
/**
 * Data asset identity for a quest. Referenced by UQuestComponent
 * instead of a bare FName, so quests are actual browsable/reference-
 * able assets (and can carry designer-facing data like display name
 * and description without touching code).
 */
UCLASS(BlueprintType)
class QUESTCORERUNTIME_API UQuestDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	// Unique gameplay id used for lookups (QuestSubsystem::ActivateQuest, etc).
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest|Info")
	FName QuestId;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Info")
	FText DisplayName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Info", meta = (MultiLine = true))
	FText Description;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Info")
	EQuestType Type = EQuestType::Main;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Info")
	int32 Priority = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Info")
	TArray<FName> Tags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Dependency")
	TArray<UQuestDefinition *> QuestDependencies;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Permission", meta = (InlineEditConditionToggle))
	bool bOverrideAutoActive = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Permission", meta = (EditCondition = "bOverrideAutoActive"))
	bool bAutoActive = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Permission", meta = (InlineEditConditionToggle))
	bool bOverrideAutoDestroy = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Permission", meta = (EditCondition = "bOverrideAutoDestroy"))
	bool bAutoDestroy = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Permission", meta = (InlineEditConditionToggle))
	bool bOverridePrerequisit = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Permission", meta = (EditCondition = "bOverridePrerequisit"))
	TArray<TSubclassOf<UQuestPrerequisite>> PrerequisitClasses;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Permission", meta = (InlineEditConditionToggle))
	bool bOverrideObjective = false;
	/**
	 * The objective types this quest expects, in order. QuestComponent's
	 * Objectives array (the actual instances) is validated against this
	 * list in-editor - see UQuestComponent::PostEditChangeProperty.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Permission", meta = (EditCondition = "bOverrideObjective"))
	TArray<TSubclassOf<UQuestObjective>> ObjectiveClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Event", meta = (Tooltip = "Set ActiveQuest when this quest completed"))
	TArray<UQuestDefinition *> OnCompleted;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Event", meta = (Tooltip = "Set ActiveQuest when this quest failed"))
	TArray<UQuestDefinition *> OnFailed;

#if WITH_EDITOR
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
