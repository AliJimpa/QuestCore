#pragma once

#include "CoreMinimal.h"
#include "System/QuestPrerequisite.h"
#include "QuestPrerequisite_QuestCompletionById.generated.h"

UCLASS(meta = (DisplayName = "QuestCompletionById"))
class QUESTCORERUNTIME_API UQuestPrerequisite_QuestCompletionById : public UQuestPrerequisite
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
				if (Executer->GetQuestSubsystem()->IsQuestCompletedById(Depended) == false)
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
