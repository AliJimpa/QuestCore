#pragma once

#include "CoreMinimal.h"
#include "System/QuestObjective.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "QuestObjective_Wait.generated.h"

/**
 * Complete after Duration seconds have passed since Begin(). Uses
 * the world's TimerManager (fires once) rather than a tick - still
 * fits the "no ticking" rule, since a single-shot timer isn't polling.
 */
UCLASS(meta = (DisplayName = "Wait"))
class QUESTCORERUNTIME_API UQuestObjective_Wait : public UQuestObjective
{
	GENERATED_BODY()

private:
	EQuestObjectiveState CachedState = EQuestObjectiveState::InProgress;
	FTimerHandle TimerHandle;

public:
	UPROPERTY(EditAnywhere, Category = "Objective")
	float Duration = 5.f;

private:
	UPROPERTY()
	TWeakObjectPtr<AActor> OwnerActor;

private:
	void HandleTimerFinished()
	{
		CachedState = EQuestObjectiveState::Done;
	}

public:
	virtual void Begin_Implementation(AActor *Owner, UQuestDefinition *Defination) override
	{
		OwnerActor = Owner;
		CachedState = EQuestObjectiveState::InProgress;

		if (UWorld *World = Owner ? Owner->GetWorld() : nullptr)
		{
			World->GetTimerManager().SetTimer(TimerHandle, this, &UQuestObjective_Wait::HandleTimerFinished, Duration, false);
		}
	}
	virtual void End_Implementation() override
	{
		if (UWorld *World = OwnerActor.IsValid() ? OwnerActor->GetWorld() : nullptr)
		{
			World->GetTimerManager().ClearTimer(TimerHandle);
		}
	}
	virtual EQuestObjectiveState GetState_Implementation() const override { return CachedState; }
	virtual float GetProgress_Implementation() const override
	{
		if (UWorld *World = OwnerActor.IsValid() ? OwnerActor->GetWorld() : nullptr)
		{
			const float Elapsed = World->GetTimerManager().GetTimerElapsed(TimerHandle);
			return Duration > 0.f ? FMath::Clamp(Elapsed / Duration, 0.f, 1.f) : 1.f;
		}
		return 0.f;
	}
	virtual FString GetObjectiveDescription_Implementation() const override { return TEXT("Wait Duration"); }
};
