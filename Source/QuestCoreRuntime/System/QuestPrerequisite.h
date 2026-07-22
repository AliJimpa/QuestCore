#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/QuestDebug.h"
#include "QuestPrerequisite.generated.h"

class UQuestComponent;

/**
 * Reuses the same Command Pattern shape as UQuestObjective, applied
 * to quest availability instead of quest progress. A Quest is
 * "available" once all of its prerequisites are satisfied.
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced, CollapseCategories)
class QUESTCORERUNTIME_API UQuestPrerequisite : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Quest|Prerequisite")
	bool IsSatisfied(const UQuestComponent *Executer) const;
	virtual bool IsSatisfied_Implementation(const UQuestComponent *Executer) const
	{
		LOG_WARNING("Empty Prerequisite!");
		return true;
	}

#if WITH_EDITOR
	UFUNCTION(BlueprintNativeEvent, Category = "Quest|Prerequisite")
	void OnVisualize(const UQuestComponent *Executer);
	virtual void OnVisualize_Implementation(const UQuestComponent *Executer) {};
#endif
};
