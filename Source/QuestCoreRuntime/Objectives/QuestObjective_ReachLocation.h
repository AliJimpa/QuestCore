#pragma once

#include "CoreMinimal.h"
#include "System/QuestObjective.h"
#include "Kismet/GameplayStatics.h"
#include "QuestObjective_ReachLocation.generated.h"

UENUM(BlueprintType)
enum class EQuestLocationSource : uint8
{
	// The quest owner (the actor Begin() was called with).
	OwnerActor,
	// A player pawn, selected by player index (splitscreen/multiplayer).
	PlayerActor,
	// A specific actor reference assigned on the objective.
	TargetActor
};

/**
 * Complete once the Source actor gets within AcceptRadius of the
 * Target actor. Both ends are resolved independently through the same
 * 3-option mode, so this can express "owner reaches a fixed actor",
 * "player 0 reaches the owner", "actor A reaches actor B", etc,
 * without extra objective classes.
 *
 * Polling-style objective - both actors are re-resolved live each time
 * GetState()/GetProgress() is called, so a moving TargetActor works
 * correctly without any extra binding.
 */
UCLASS(meta = (DisplayName = "Reach Location"))
class QUESTCORERUNTIME_API UQuestObjective_ReachLocation : public UQuestObjective
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float AcceptRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = "Source")
	EQuestLocationSource SourceMode = EQuestLocationSource::OwnerActor;
	UPROPERTY(EditAnywhere, Category = "Source", meta = (EditCondition = "SourceMode==EQuestLocationSource::PlayerActor", EditConditionHides))
	int32 SourcePlayerIndex = 0;
	UPROPERTY(EditAnywhere, Category = "Source", meta = (EditCondition = "SourceMode==EQuestLocationSource::TargetActor", EditConditionHides))
	TObjectPtr<AActor> SourceActor;

	UPROPERTY(EditAnywhere, Category = "Target")
	EQuestLocationSource TargetMode = EQuestLocationSource::TargetActor;
	UPROPERTY(EditAnywhere, Category = "Target", meta = (EditCondition = "TargetMode==EQuestLocationSource::PlayerActor", EditConditionHides))
	int32 TargetPlayerIndex = 0;
	UPROPERTY(EditAnywhere, Category = "Target", meta = (EditCondition = "TargetMode==EQuestLocationSource::TargetActor", EditConditionHides))
	TObjectPtr<AActor> TargetActor;

private:
	// Resolves a mode to a live actor pointer - called every poll, so
	// PlayerActor/TargetActor references always reflect their current state.
	AActor *ResolveActor(EQuestLocationSource Mode, int32 PlayerIndex, bool IsSource) const
	{
		AActor *Owner = GetOwner();
		switch (Mode)
		{
		case EQuestLocationSource::OwnerActor:
			return Owner;

		case EQuestLocationSource::PlayerActor:
		{
			// Needs a world context - OwnerActor is the only actor we're
			// guaranteed to have, so resolve the world through it.
			UWorld *World = Owner ? Owner->GetWorld() : nullptr;
			return World ? UGameplayStatics::GetPlayerPawn(World, PlayerIndex) : nullptr;
		}

		case EQuestLocationSource::TargetActor:
			return IsSource ? SourceActor : TargetActor;
		}

		return nullptr;
	}

public:
	virtual EQuestObjectiveState GetState_Implementation() const override
	{
		AActor *Source = ResolveActor(SourceMode, SourcePlayerIndex, true);
		AActor *Target = ResolveActor(TargetMode, TargetPlayerIndex, false);

		// Either side unresolved (not spawned yet, player disconnected, etc)
		// just means "not there yet", not a failure - keep waiting.
		if (!Source || !Target)
		{
			return EQuestObjectiveState::InProgress;
		}

		const float Distance = FVector::Dist(Source->GetActorLocation(), Target->GetActorLocation());
		return Distance <= AcceptRadius ? EQuestObjectiveState::Done : EQuestObjectiveState::InProgress;
	}
	virtual float GetProgress_Implementation() const override
	{
		AActor *Source = ResolveActor(SourceMode, SourcePlayerIndex, true);
		AActor *Target = ResolveActor(TargetMode, TargetPlayerIndex, false);

		if (!Source || !Target)
		{
			return 0.f;
		}

		// Still just a 0/1 flag, not a real distance-based ramp - see note
		// on the original version about needing a start point for that.
		const float Distance = FVector::Dist(Source->GetActorLocation(), Target->GetActorLocation());
		return Distance <= AcceptRadius ? 1.f : 0.f;
	}
	virtual FString GetObjectiveDescription_Implementation() const override { return TEXT("Reach Location"); }
};