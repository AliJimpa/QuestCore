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
	UPROPERTY(EditAnywhere, Category = "Objective")
	EQuestObjectiveState Result = EQuestObjectiveState::Done;

private:
	void HandleTimerFinished()
	{
		CachedState = Result;
	}

public:
	virtual void Begin_Implementation() override
	{
		CachedState = EQuestObjectiveState::InProgress;
		World->GetTimerManager().SetTimer(TimerHandle, this, &UQuestObjective_Wait::HandleTimerFinished, Duration, false);
	}
	virtual void End_Implementation() override
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}
	virtual EQuestObjectiveState GetState_Implementation() const override { return CachedState; }
	virtual float GetProgress_Implementation() const override
	{
		if (CachedState == EQuestObjectiveState::InProgress)
		{
			const float Elapsed = World->GetTimerManager().GetTimerElapsed(TimerHandle);
			return Duration > 0.f ? FMath::Clamp(Elapsed / Duration, 0.f, 1.f) : 1.f;
		}
		if (CachedState == EQuestObjectiveState::Done)
			return 1;
		if (CachedState == EQuestObjectiveState::Failed)
			return 0;
		if (CachedState == EQuestObjectiveState::Canceld)
			return 0;
		return -1;
	}
	virtual FString GetObjectiveDescription_Implementation() const override { return TEXT("Wait Duration"); }
};
