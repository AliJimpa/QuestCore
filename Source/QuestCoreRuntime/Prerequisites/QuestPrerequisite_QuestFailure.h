#pragma once

#include "CoreMinimal.h"
#include "System/QuestPrerequisite.h"
#include "QuestPrerequisite_QuestFailure.generated.h"

UCLASS(meta = (DisplayName = "QuestFailure"))
class QUESTCORERUNTIME_API UQuestPrerequisite_QuestFailure : public UQuestPrerequisite
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UQuestDefinition *> QuestDependencies;

public:
	virtual bool IsSatisfied_Implementation(const UQuestComponent *Executer) const override
	{
		if (QuestDependencies.Num() > 0)
		{
			for (UQuestDefinition *Depended : QuestDependencies)
			{
				if (Depended == nullptr)
					continue;
				if (Executer->GetQuestSubsystem()->IsQuestFailedByDefinition(Depended) == false)
				{
					LOG_WARNING("Quest(%s) need to completed first, because it depends on quest", *Depended->GetName());
					return false;
				}
			}
		}
		else
		{
			return false;
		}
		return true;
	}
};
