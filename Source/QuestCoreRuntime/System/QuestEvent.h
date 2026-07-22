#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestEvent.generated.h"

class UQuestComponent;

UCLASS(Abstract, EditInlineNew, DefaultToInstanced, BlueprintType, Blueprintable, CollapseCategories)
class QUESTCORERUNTIME_API UQuestEvent : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Quest|Event")
	void Execute(const UQuestComponent *Executer);
	virtual void Execute_Implementation(const UQuestComponent *Executer) {}

#if WITH_EDITOR
	UFUNCTION(BlueprintNativeEvent, Category = "Quest|Event")
	void OnVisualize(const UQuestComponent *Executer);
	virtual void OnVisualize_Implementation(const UQuestComponent *Executer) {};
#endif
};
