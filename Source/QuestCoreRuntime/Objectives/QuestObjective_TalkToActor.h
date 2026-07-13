#pragma once

#include "CoreMinimal.h"
#include "System/QuestObjective.h"
#include "QuestObjective_TalkToActor.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDialogueFinished, AActor * /*Player*/, AActor * /*NPC*/);

/**
 * Complete once the quest owner finishes a dialogue with TargetNPC.
 * Event-driven, one-shot - no progress bar needed for this one.
 */
UCLASS(meta = (DisplayName = "Talk To Actor"))
class QUESTCORERUNTIME_API UQuestObjective_TalkToActor : public UQuestObjective
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Quest")
	TSoftObjectPtr<AActor> TargetNPC;

	virtual void Begin_Implementation(AActor *Owner, UQuestDefinition *Defination) override
	{
		OwnerActor = Owner;
		CachedState = EQuestObjectiveState::InProgress;
		DialogueDelegateHandle = OnDialogueFinished.AddUObject(this, &UQuestObjective_TalkToActor::HandleDialogueFinished);
	}
	virtual void End_Implementation() override
	{
		OnDialogueFinished.Remove(DialogueDelegateHandle);
	}
	virtual EQuestObjectiveState GetState_Implementation() const override { return CachedState; }

#if WITH_EDITOR
	virtual FString GetObjectiveDescription() const override { return TEXT("Talk To Actor"); }
#endif

	// Broadcast this from your dialogue system when a conversation ends.
	static FOnDialogueFinished OnDialogueFinished;

private:
	void HandleDialogueFinished(AActor *Player, AActor *NPC)
	{
		if (OwnerActor.IsValid() && Player != OwnerActor.Get())
		{
			return;
		}
		if (NPC != TargetNPC.Get())
		{
			return;
		}

		CachedState = EQuestObjectiveState::Done;
	}

	UPROPERTY()
	TWeakObjectPtr<AActor> OwnerActor;

	EQuestObjectiveState CachedState = EQuestObjectiveState::InProgress;
	FDelegateHandle DialogueDelegateHandle;
};
