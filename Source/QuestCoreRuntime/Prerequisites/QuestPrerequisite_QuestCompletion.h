#pragma once

#include "CoreMinimal.h"
#include "System/QuestPrerequisite.h"
#include "QuestPrerequisite_QuestCompletion.generated.h"

UCLASS(meta = (DisplayName = "QuestCompletion"))
class QUESTCORERUNTIME_API UQuestPrerequisite_QuestCompletion : public UQuestPrerequisite
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
				if (Executer->GetQuestSubsystem()->IsQuestCompletedByDefinition(Depended) == false)
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
