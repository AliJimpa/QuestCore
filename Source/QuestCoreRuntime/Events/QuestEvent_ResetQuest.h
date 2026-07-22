#pragma once

#include "CoreMinimal.h"
#include "System/QuestEvent.h"
#include "System/QuestComponent.h"
#include "SubSystem/QuestSubsystem.h"
#include "QuestEvent_ResetQuest.generated.h"

class UQuestDefinition;

UCLASS(DisplayName = "Reset Quest")
class QUESTCORERUNTIME_API UQuestEvent_ResetQuest : public UQuestEvent
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
    TArray<TObjectPtr<UQuestDefinition>> QuestsToReset;

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

        for (const UQuestDefinition *Quest : QuestsToReset)
        {
            if (Quest)
            {
                Subsystem->ResetQuest(Quest);
            }
        }
    }
};