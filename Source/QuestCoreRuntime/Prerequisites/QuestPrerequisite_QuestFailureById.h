#pragma once

#include "CoreMinimal.h"
#include "System/QuestPrerequisite.h"
#include "QuestPrerequisite_QuestFailureById.generated.h"

UCLASS(meta = (DisplayName = "QuestFailureById"))
class QUESTCORERUNTIME_API UQuestPrerequisite_QuestFailureById : public UQuestPrerequisite
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FName> QuestDependencies;

public:
	virtual bool IsSatisfied_Implementation(const UQuestComponent *Executer) const override
	{
		if (QuestDependencies.Num() > 0)
		{
			for (FName Depended : QuestDependencies)
			{
				if (Depended == NAME_None)
					continue;
				if (Executer->GetQuestSubsystem()->IsQuestFailedById(Depended) == false)
				{
					LOG_WARNING("Quest(%s) need to completed first, because it depends on quest", *Depended.ToString());
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
