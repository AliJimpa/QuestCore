#pragma once

#include "CoreMinimal.h"
#include "Factories/BlueprintFactory.h"
#include "System/QuestObjective.h"
#include "QuestObjectiveBlueprintFactory.generated.h"

UCLASS()
class QUESTCOREEDITOR_API UQuestObjectiveBlueprintFactory : public UBlueprintFactory
{
    GENERATED_BODY()

public:
    UQuestObjectiveBlueprintFactory()
    {
        bCreateNew = true;
        bEditAfterNew = true;
        SupportedClass = UBlueprint::StaticClass();
        ParentClass = UQuestObjective::StaticClass();
    }

    virtual FText GetDisplayName() const override
    {
        return NSLOCTEXT("QuestCoreEditor", "QuestObjectiveFactory", "Objective");
    }

    // Skip the generic "pick a parent class" dialog — always use UQuestObjective.
    virtual bool ConfigureProperties() override
    {
        ParentClass = UQuestObjective::StaticClass();
        return true;
    }
};