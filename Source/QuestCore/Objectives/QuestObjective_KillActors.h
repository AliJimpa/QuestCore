#pragma once

#include "CoreMinimal.h"
#include "QuestObjective_Event.h"
#include "QuestObjective_KillActors.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnActorKilled, AActor* /*KilledActor*/);

/**
 * Example Event objective: complete once N actors of a given class
 * have been killed. Shows the intended pattern - Begin() binds to a
 * gameplay event, the handler updates progress, End() unbinds.
 * Wire ExternalActorKilledDelegate up to your own kill-event source
 * (a GameplayMessage, an ActorComponent delegate, whatever you use).
 */
UCLASS()
class QUESTCORE_API UQuestObjective_KillActors : public UQuestObjective_Event
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Quest")
	TSubclassOf<AActor> TargetClass;

	UPROPERTY(EditAnywhere, Category = "Quest")
	int32 RequiredKills = 1;

	virtual void Begin_Implementation() override;
	virtual void End_Implementation() override;
	virtual float GetProgress_Implementation() const override;

private:
	void HandleActorKilled(AActor* KilledActor);

	UPROPERTY()
	int32 CurrentKills = 0;

	FDelegateHandle KillDelegateHandle;

public:
	// Demo hook - broadcast this from wherever your kill logic lives
	// (a damage handler, a death component, etc). Replace with your
	// own project's kill event source.
	static FOnActorKilled OnActorKilled;
};
