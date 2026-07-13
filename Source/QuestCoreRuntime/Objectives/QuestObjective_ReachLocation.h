#pragma once

#include "CoreMinimal.h"
#include "System/QuestObjective.h"
#include "QuestObjective_ReachLocation.generated.h"

/**
 * Complete once the quest owner gets within AcceptRadius of TargetLocation.
 * Polling-style objective - no event binding needed, GetState() just
 * measures live distance each time the Quest asks.
 */
UCLASS(meta = (DisplayName = "Reach Location"))
class QUESTCORERUNTIME_API UQuestObjective_ReachLocation : public UQuestObjective
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Quest")
	FVector TargetLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, Category = "Quest")
	float AcceptRadius = 150.f;

private:
	UPROPERTY()
	TWeakObjectPtr<AActor> OwnerActor;

public:
	virtual void Begin_Implementation(AActor *Owner, UQuestDefinition *Defination) override
	{
		OwnerActor = Owner;
	}
	virtual EQuestObjectiveState GetState_Implementation() const override
	{
		if (!OwnerActor.IsValid())
		{
			return EQuestObjectiveState::Canceld;
		}

		const float Distance = FVector::Dist(OwnerActor->GetActorLocation(), TargetLocation);
		return Distance <= AcceptRadius ? EQuestObjectiveState::Done : EQuestObjectiveState::InProgress;
	}
	virtual float GetProgress_Implementation() const override
	{
		if (!OwnerActor.IsValid())
		{
			return 0.f;
		}

		// Not a great progress metric without a start point, but gives a
		// rough sense of "getting closer" for a HUD without extra state.
		const float Distance = FVector::Dist(OwnerActor->GetActorLocation(), TargetLocation);
		return Distance <= AcceptRadius ? 1.f : 0.f;
	}

#if WITH_EDITOR
	virtual FString GetObjectiveDescription() const override { return TEXT("Reach Location"); }
#endif
};
