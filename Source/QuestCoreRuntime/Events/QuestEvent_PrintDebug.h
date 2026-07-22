#pragma once

#include "CoreMinimal.h"
#include "Engine/QuestDebug.h"
#include "System/QuestEvent.h"
#include "System/QuestComponent.h"
#include "SubSystem/QuestSubsystem.h"
#include "QuestEvent_PrintDebug.generated.h"

class UQuestDefinition;

UCLASS(DisplayName = "PrintDebug")
class QUESTCORERUNTIME_API UQuestEvent_PrintDebug : public UQuestEvent
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, Category = "Message")
    FString Message;
    UPROPERTY(EditAnywhere, Category = "Message")
    FColor Color = FColor::Green;
    UPROPERTY(EditAnywhere, Category = "Message", meta = (ClampMin = "0.0"))
    float Duration = 5.f;
    UPROPERTY(EditAnywhere, Category = "Output")
    bool bPrintToScreen = true;
    UPROPERTY(EditAnywhere, Category = "Output")
    bool bPrintToLog = true;
    UPROPERTY(EditAnywhere, Category = "Output")
    bool bIncludeQuestContext = true;
    UPROPERTY(EditAnywhere, Category = "Output")
    FName LogCategory = TEXT("Quest");

public:
    virtual void Execute_Implementation(const UQuestComponent *Executer) override
    {
        FString Prefix;

        if (bIncludeQuestContext && Executer)
        {
            const FString ComponentName = Executer->GetName();

            const FString DefinitionName =
                Executer->GetDefinition()
                    ? Executer->GetDefinition()->GetName()
                    : TEXT("None");

            Prefix = FString::Printf(
                TEXT("[%s] %s (%s): "),
                *LogCategory.ToString(),
                *ComponentName,
                *DefinitionName);
        }

        const FString FinalMessage = Prefix + Message;

        if (bPrintToScreen && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                Duration,
                Color,
                FinalMessage);
        }

        if (bPrintToLog)
        {
            LOG("%s", *FinalMessage);
        }
    }
};