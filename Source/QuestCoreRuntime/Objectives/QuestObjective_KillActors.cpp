#include "QuestObjective_KillActors.h"

FOnActorKilled UQuestObjective_KillActors::OnActorKilled;

void UQuestObjective_KillActors::Begin_Implementation(AActor *Owner, UQuestDefinition *Defination)
{
	CurrentKills = 0;
	KillDelegateHandle = OnActorKilled.AddUObject(this, &UQuestObjective_KillActors::HandleActorKilled);
}

void UQuestObjective_KillActors::End_Implementation()
{
	OnActorKilled.Remove(KillDelegateHandle);
}

float UQuestObjective_KillActors::GetProgress_Implementation() const
{
	return RequiredKills > 0 ? static_cast<float>(CurrentKills) / static_cast<float>(RequiredKills) : 0.f;
}

void UQuestObjective_KillActors::HandleActorKilled(AActor* KilledActor)
{
	if (!KilledActor || !KilledActor->IsA(TargetClass))
	{
		return;
	}

	CurrentKills++;
	if (CurrentKills >= RequiredKills)
	{
		NotifyDone();
	}
}
