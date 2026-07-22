#pragma once

#include "CoreMinimal.h"
#include "System/QuestEvent.h"
#include "System/QuestComponent.h"
#include "SubSystem/QuestSubsystem.h"
#include "QuestEvent_StartQuest.generated.h"

class UQuestDefinition;

UCLASS(DisplayName = "Start Quest")
class QUESTCORERUNTIME_API UQuestEvent_StartQuest : public UQuestEvent
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
    TArray<TObjectPtr<UQuestDefinition>> QuestsToStart;

public:
    virtual void Execute_Implementation(const UQuestComponent *Executer) override
    {
        if (!Executer)
        {
            return;
        }

        UQuestSubsystem *Subsystem = Executer->GetQuestSubsystem();
        if (!Subsystem)
        {
            return;
        }

        for (const UQuestDefinition *Quest : QuestsToStart)
        {
            if (Quest)
            {
                Subsystem->StartQuest(Quest);
            }
        }
    }
};