#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestDefinition.generated.h"

class UQuestObjective;
class UQuestPrerequisite;
class UQuestEvent;

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
	UPROPERTY(VisibleAnywhere, Category = "Quest|Info")
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

	UPROPERTY(EditAnywhere, Instanced, Category = "Quest|Setting")
	TArray<TObjectPtr<UQuestPrerequisite>> Prerequisites;
	UPROPERTY(EditAnywhere, Instanced, Category = "Quest|Setting")
	TArray<TObjectPtr<UQuestObjective>> Objectives;
	UPROPERTY(EditAnywhere, Instanced, Category = "Quest|Events")
	TArray<TObjectPtr<UQuestEvent>> OnCompleted;
	UPROPERTY(EditAnywhere, Instanced, Category = "Quest|Events")
	TArray<TObjectPtr<UQuestEvent>> OnFailed;

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
