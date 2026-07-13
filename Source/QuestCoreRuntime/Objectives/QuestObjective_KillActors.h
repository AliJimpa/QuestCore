#pragma once

#include "CoreMinimal.h"
#include "System/QuestObjective.h"
#include "QuestObjective_KillActors.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnActorKilled, AActor * /*Killer*/, AActor * /*KilledActor*/);
// FOnActorKilled UQuestObjective_KillActors::OnActorKilled;

/**
 * Complete once RequiredKills actors of TargetClass have been killed
 * by the quest owner. Event-driven - binds in Begin(), unbinds in
 * End(). Replace the static OnActorKilled hook with your project's
 * real kill/death event source.
 */
UCLASS(meta = (DisplayName = "Kill Actors"))
class QUESTCORERUNTIME_API UQuestObjective_KillActors : public UQuestObjective
{
	GENERATED_BODY()

private:
	FDelegateHandle KillDelegateHandle;

public:
	UPROPERTY(EditAnywhere, Category = "Quest")
	TSubclassOf<AActor> TargetClass;

	UPROPERTY(EditAnywhere, Category = "Quest")
	int32 RequiredKills = 1;

	virtual void Begin_Implementation(AActor *Owner, UQuestDefinition *Defination) override
	{
		OwnerActor = Owner;
		CurrentKills = 0;
		CachedState = EQuestObjectiveState::InProgress;
		KillDelegateHandle = OnActorKilled.AddUObject(this, &UQuestObjective_KillActors::HandleActorKilled);
	}
	virtual void End_Implementation() override
	{
		OnActorKilled.Remove(KillDelegateHandle);
	}
	virtual EQuestObjectiveState GetState_Implementation() const override { return CachedState; }
	virtual float GetProgress_Implementation() const override
	{
		return RequiredKills > 0 ? static_cast<float>(CurrentKills) / static_cast<float>(RequiredKills) : 1.f;
	}

#if WITH_EDITOR
	virtual FString GetObjectiveDescription() const override { return TEXT("Kill Actors"); }
#endif

	// Broadcast this from wherever your kill logic lives (damage
	// handler, death component, etc).
	static FOnActorKilled OnActorKilled;

private:
	void HandleActorKilled(AActor *Killer, AActor *KilledActor)
	{
		// Only count kills made by the quest owner, on the right actor class.
		if (!KilledActor || !KilledActor->IsA(TargetClass))
		{
			return;
		}
		if (OwnerActor.IsValid() && Killer != OwnerActor.Get())
		{
			return;
		}

		CurrentKills++;
		if (CurrentKills >= RequiredKills)
		{
			CachedState = EQuestObjectiveState::Done;
		}
	}

	UPROPERTY()
	TWeakObjectPtr<AActor> OwnerActor;

	UPROPERTY()
	int32 CurrentKills = 0;

	EQuestObjectiveState CachedState = EQuestObjectiveState::InProgress;
};
