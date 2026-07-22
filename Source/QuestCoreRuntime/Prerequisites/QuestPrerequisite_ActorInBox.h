#pragma once

#include "CoreMinimal.h"
#include "System/QuestPrerequisite.h"
#include "System/QuestComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "QuestPrerequisite_ActorInBox.generated.h"

UCLASS(meta = (DisplayName = "ActorInBox"))
class QUESTCORERUNTIME_API UQuestPrerequisite_ActorInBox : public UQuestPrerequisite
{
    GENERATED_BODY()

public:
    /** Center of the box relative to the quest owner. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box")
    FVector RelativeLocation = FVector::ZeroVector;

    /** Half-size of the box. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box")
    FVector BoxExtent = FVector(100.f);

    /** Actor must have this tag. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box")
    FName TargetTag;

    virtual bool IsSatisfied_Implementation(const UQuestComponent *Executer) const override
    {
        if (!Executer)
        {
            return false;
        }

        AActor *Owner = Executer->GetOwner();
        if (!Owner)
        {
            return false;
        }

        UWorld *World = Owner->GetWorld();
        if (!World)
        {
            return false;
        }

        const FVector Center = Owner->GetActorLocation() + RelativeLocation;

        TArray<FOverlapResult> Results;

        const bool bHit = World->OverlapMultiByObjectType(Results, Center, FQuat::Identity, FCollisionObjectQueryParams::AllDynamicObjects, FCollisionShape::MakeBox(BoxExtent));

#if ENABLE_DRAW_DEBUG
        DrawDebugBox(World, Center, BoxExtent, FColor::Green, false, 1.f);
#endif

        if (!bHit)
        {
            return false;
        }

        for (const FOverlapResult &Result : Results)
        {
            AActor *Actor = Result.GetActor();

            if (IsValid(Actor) && Actor->ActorHasTag(TargetTag))
            {
                return true;
            }
        }

        return false;
    }

#if WITH_EDITOR
    virtual void OnVisualize_Implementation(const UQuestComponent *Executer) override
    {
        UWorld *World = Executer->GetWorld();

        if (!World)
            return;

        DrawDebugBox(
            World,
            Executer->GetOwner()->GetActorLocation(),
            FVector(100, 100, 100),
            FColor::Green,
            false,
            5.f);
    };
#endif
};