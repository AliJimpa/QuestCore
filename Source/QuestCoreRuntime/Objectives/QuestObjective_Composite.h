#pragma once

#include "CoreMinimal.h"
#include "System/QuestObjective.h"
#include "QuestObjective_Composite.generated.h"

UENUM(BlueprintType)
enum class EQuestCompositeMode : uint8
{
	// Done only once every child objective is Done.
	RequireAll,
	// Done as soon as any single child objective is Done.
	RequireAny
};

/**
 * Groups several objectives behind one UQuestObjective interface
 * (Composite Pattern). Useful when a single "step" in a quest is
 * really a small AND/OR combination - e.g. "kill the boss OR defuse
 * the bomb" as one objective, instead of restructuring the quest's
 * order groups to express it.
 */
UCLASS(meta = (DisplayName = "Composite"))
class QUESTCORERUNTIME_API UQuestObjective_Composite : public UQuestObjective
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Quest")
	FString Description = TEXT("Composite Objective");
	UPROPERTY(EditAnywhere, Instanced, Category = "Quest")
	TArray<TObjectPtr<UQuestObjective>> ChildObjectives;
	UPROPERTY(EditAnywhere, Category = "Quest")
	EQuestCompositeMode Mode = EQuestCompositeMode::RequireAll;

protected:
	virtual void Construction(AActor *Owner, UQuestDefinition *Defination) override
	{
		Super::Construction(Owner, Defination);
		for (UQuestObjective *Child : ChildObjectives)
		{
			if (Child)
			{
				Child->Construction(Owner, Defination);
			}
		}
	}

public:
	virtual void Begin_Implementation() override
	{
		for (UQuestObjective *Child : ChildObjectives)
		{
			if (Child)
			{
				Child->Begin();
			}
		}
	}
	virtual void End_Implementation() override
	{
		for (UQuestObjective *Child : ChildObjectives)
		{
			if (Child)
			{
				Child->End();
			}
		}
	}
	virtual EQuestObjectiveState GetState_Implementation() const override
	{
		if (ChildObjectives.Num() == 0)
		{
			// Vacuous case: RequireAll on an empty set is trivially satisfied,
			// RequireAny on an empty set can never be satisfied.
			return Mode == EQuestCompositeMode::RequireAll ? EQuestObjectiveState::Done : EQuestObjectiveState::InProgress;
		}

		bool bAnyFailed = false;
		bool bAnyDone = false;
		bool bAllDone = true;

		for (const UQuestObjective *Child : ChildObjectives)
		{
			if (!Child)
			{
				continue;
			}

			const EQuestObjectiveState ChildState = Child->GetState();
			if (ChildState == EQuestObjectiveState::Failed)
			{
				bAnyFailed = true;
			}
			else if (ChildState == EQuestObjectiveState::Done)
			{
				bAnyDone = true;
			}
			else
			{
				bAllDone = false;
			}
		}

		// A failed child only fails the whole composite in RequireAll mode -
		// in RequireAny mode a failure just means that branch is out of the
		// running, not that the composite itself failed.
		if (bAnyFailed && Mode == EQuestCompositeMode::RequireAll)
		{
			return EQuestObjectiveState::Failed;
		}

		if (Mode == EQuestCompositeMode::RequireAll)
		{
			return bAllDone ? EQuestObjectiveState::Done : EQuestObjectiveState::InProgress;
		}

		return bAnyDone ? EQuestObjectiveState::Done : EQuestObjectiveState::InProgress;
	}
	virtual float GetProgress_Implementation() const override
	{
		if (ChildObjectives.Num() == 0)
		{
			return 1.f;
		}

		float Total = 0.f;
		int32 Count = 0;

		for (const UQuestObjective *Child : ChildObjectives)
		{
			if (!Child)
			{
				continue;
			}
			Total += Child->GetProgress();
			Count++;
		}

		return Count > 0 ? Total / Count : 1.f;
	}
	virtual FString GetObjectiveDescription_Implementation() const override { return Description; }
};