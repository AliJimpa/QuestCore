#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestPrerequisite.generated.h"

/**
 * Reuses the same Command Pattern shape as UQuestObjective, applied
 * to quest availability instead of quest progress. A Quest is
 * "available" once all of its prerequisites are satisfied.
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class QUESTCORERUNTIME_API UQuestPrerequisite : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Quest|Prerequisite")
	bool IsSatisfied(AActor *Owner, UQuestDefinition *Defination) const;
	virtual bool IsSatisfied_Implementation(AActor *Owner, UQuestDefinition *Defination) const { return true; }
};
