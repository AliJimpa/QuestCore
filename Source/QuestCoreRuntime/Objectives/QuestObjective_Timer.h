#pragma once

#include "CoreMinimal.h"
#include "System/QuestObjective.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "QuestObjective_Timer.generated.h"

/**
 * Complete after Duration seconds have passed since Begin(). Uses
 * the world's TimerManager (fires once) rather than a tick - still
 * fits the "no ticking" rule, since a single-shot timer isn't polling.
 */
UCLASS(meta = (DisplayName = "Wait Duration"))
class QUESTCORERUNTIME_API UQuestObjective_Timer : public UQuestObjective
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Quest")
	float Duration = 5.f;

	virtual void Begin_Implementation(AActor *Owner, UQuestDefinition *Defination) override
	{
		OwnerActor = Owner;
		CachedState = EQuestObjectiveState::InProgress;

		if (UWorld *World = Owner ? Owner->GetWorld() : nullptr)
		{
			World->GetTimerManager().SetTimer(TimerHandle, this, &UQuestObjective_Timer::HandleTimerFinished, Duration, false);
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

#if WITH_EDITOR
	virtual FString GetObjectiveDescription() const override { return TEXT("Wait Duration"); }
#endif

private:
	void HandleTimerFinished()
	{
		CachedState = EQuestObjectiveState::Done;
	}

	UPROPERTY()
	TWeakObjectPtr<AActor> OwnerActor;

	EQuestObjectiveState CachedState = EQuestObjectiveState::InProgress;
	FTimerHandle TimerHandle;
};
