#include "QuestComponent.h"
#include "SubSystem/QuestSubsystem.h"
#include "QuestObjective.h"
#include "Engine/QuestDebug.h"

void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	if (QuestDefinition != nullptr)
	{
		for (UQuestObjective *Objective : Objectives)
		{
			Objective->Construction(this, QuestDefinition);
		}
	}
	else
	{
		LOG_ERROR("QuestDefinition is not valid");
		return;
	}

	GetQuestSubsystem()->RegisterQuest(this);

	if (bAutoActive && State == EQuestState::NotStarted)
	{
		ActivateQuest();
	}
}

void UQuestComponent::ApplyLoadedState(EQuestState SavedState)
{
	switch (SavedState)
	{
	case EQuestState::Active:
		SetState(EQuestState::Active);
		GetQuestSubsystem()->SubmitQuestActivation(this, true);
		break;
	case EQuestState::Completed:
		State = EQuestState::Completed;
		if (bAutoDestroy)
		{
			GetOwner()->Destroy();
		}
		break;
	case EQuestState::Failed:
		State = EQuestState::Failed;
		if (bAutoDestroy)
		{
			GetOwner()->Destroy();
		}
		break;
	}
}

UQuestSubsystem *UQuestComponent::GetQuestSubsystem() const
{
	if (!CachedSubsystem.IsValid())
	{
		if (UQuestSubsystem *Subsystem = GetWorld() ? GetWorld()->GetSubsystem<UQuestSubsystem>() : nullptr)
		{
			CachedSubsystem = Subsystem;
		}
		else
		{
			LOG_ERROR("Can't find UQuestSubsystem for Register");
			return nullptr;
		}
	}

	return CachedSubsystem.Get();
}

void UQuestComponent::BeginObjectives()
{
	for (UQuestObjective *Objective : Objectives)
	{
		Objective->Begin();
	}
}

void UQuestComponent::EndObjectives()
{
	for (UQuestObjective *Objective : Objectives)
	{
		Objective->End();
	}
}

void UQuestComponent::SetState(const EQuestState NewState)
{
	const EQuestState LastState = State;
	State = NewState;
	GetQuestSubsystem()->OnQuestStateChanged.Broadcast(this);
	switch (State)
	{
	case EQuestState::NotStarted:
		if (LastState == EQuestState::Active)
			EndObjectives();
		break;
	case EQuestState::Active:
		BeginObjectives();
		break;
	case EQuestState::Completed:
		EndObjectives();
		OnQuestCompleted.Broadcast(this);
		if (QuestDefinition != nullptr)
		{
			for (UQuestDefinition *QuestDef : QuestDefinition->OnCompleted)
			{
				if (UQuestComponent *QuestComp = GetQuestSubsystem()->FindQuestByDefinition(QuestDef))
				{
					QuestComp->ActivateQuest();
				}
			}
			if (QuestDefinition->bAutoSave)
			{
				GetQuestSubsystem()->SaveQuestData();
			}
		}
		if (bAutoDestroy)
		{
			GetOwner()->Destroy();
		}
		break;
	case EQuestState::Failed:
		EndObjectives();
		OnQuestFailed.Broadcast(this);
		if (QuestDefinition != nullptr)
		{
			for (UQuestDefinition *QuestDef : QuestDefinition->OnFailed)
			{
				if (UQuestComponent *QuestComp = GetQuestSubsystem()->FindQuestByDefinition(QuestDef))
				{
					QuestComp->ActivateQuest();
				}
			}
			if (QuestDefinition->bAutoSave)
			{
				GetQuestSubsystem()->SaveQuestData();
			}
		}
		if (bAutoDestroy)
		{
			GetOwner()->Destroy();
		}
		break;
	}
}

bool UQuestComponent::ActivateQuest()
{
	if (State == EQuestState::Active)
	{
		LOG("Quest already is Active.");
		return false;
	}

	if (!ArePrerequisitesSatisfied())
	{
		LOG_WARNING("Prerequisites not Satisfied.");
		return false;
	}

	SetState(EQuestState::Active);
	GetQuestSubsystem()->SubmitQuestActivation(this, true);

	return true;
}

bool UQuestComponent::DeactivateQuest()
{
	if (State == EQuestState::NotStarted)
	{
		LOG("Quest already is Deactive.");
		return false;
	}

	SetState(EQuestState::NotStarted);
	GetQuestSubsystem()->SubmitQuestActivation(this, false);

	return true;
}

bool UQuestComponent::ArePrerequisitesSatisfied() const
{
	if (QuestDefinition != nullptr)
	{
		if (QuestDefinition->QuestDependencies.Num() > 0)
		{
			for (UQuestDefinition *Depended : QuestDefinition->QuestDependencies)
			{
				if (Depended == nullptr)
					continue;
				if (GetQuestSubsystem()->IsQuestCompletedByDefinition(Depended) == false)
				{
					LOG_WARNING("Quest(%s) need to Complite first there is dependency Quest", *Depended->GetName());
					return false;
				}
			}
		}
	}
	for (const UQuestPrerequisite *Prerequisite : Prerequisites)
	{
		if (!Prerequisite || !Prerequisite->IsSatisfied(GetOwner(), QuestDefinition))
		{
			return false;
		}
	}
	return true;
}

void UQuestComponent::UpdateQuest()
{
	if (State != EQuestState::Active)
	{
		return;
	}

	bool bAnyFailed = false;
	bool bAllDone = true;
	for (const UQuestObjective *Objective : Objectives)
	{
		const EQuestObjectiveState ObjState = Objective->GetState();
		if (ObjState == EQuestObjectiveState::Failed)
		{
			bAnyFailed = true;
			break;
		}
		if (ObjState != EQuestObjectiveState::Done)
		{
			bAllDone = false;
		}
	}

	OnQuestUpdated.Broadcast(this);

	if (bAnyFailed)
	{
		SetState(EQuestState::Failed);
	}
	else
	{
		if (bAllDone)
		{
			SetState(EQuestState::Completed);
		}
	}
}

float UQuestComponent::GetProgress() const
{
	float Total = 0.f;
	for (const UQuestObjective *Objective : Objectives)
	{
		Total += Objective->GetProgress();
	}
	return Total / Objectives.Num();
}

UQuestObjective *UQuestComponent::GetCurrentObjective() const
{
	for (UQuestObjective *Objective : Objectives)
	{
		if (Objective && Objective->GetState() == EQuestObjectiveState::InProgress)
		{
			return Objective;
		}
	}
	return nullptr;
}

int32 UQuestComponent::GetCurrentObjectiveIndex() const
{
	int32 Reesult = 0;
	for (UQuestObjective *Objective : Objectives)
	{
		if (Objective && Objective->GetState() == EQuestObjectiveState::InProgress)
		{
			return Reesult;
		}
		Reesult++;
	}
	return -1;
}

void UQuestComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DeactivateQuest();

	GetQuestSubsystem()->UnregisterQuest(this);

	Super::EndPlay(EndPlayReason);
}

#if WITH_EDITOR
void UQuestComponent::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (QuestDefinition != nullptr)
	{
		if (QuestDefinition->bOverrideAutoActive)
		{
			if (bAutoActive != QuestDefinition->bAutoActive)
			{
				const FString OwnerName = GetOwner() ? GetOwner()->GetName() : GetName();
				const FString Message = FString::Printf(
					TEXT("[%s] AutoActive does not match QuestDefinition '%s' (Should be  %s)."),
					*OwnerName, *QuestDefinition->GetName(), QuestDefinition->bAutoActive ? TEXT("True") : TEXT("False"));

				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, Message);
				}
			}
		}

		if (QuestDefinition->bOverrideAutoDestroy)
		{
			if (bAutoDestroy != QuestDefinition->bAutoDestroy)
			{
				const FString OwnerName = GetOwner() ? GetOwner()->GetName() : GetName();
				const FString Message = FString::Printf(
					TEXT("[%s] AutoDestroy does not match QuestDefinition '%s' (Should be  %s)."),
					*OwnerName, *QuestDefinition->GetName(), QuestDefinition->bAutoDestroy ? TEXT("True") : TEXT("False"));

				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, Message);
				}
			}
		}

		if (QuestDefinition->bOverrideObjective)
		{
			const TArray<TSubclassOf<UQuestObjective>> &ExpectedClasses = QuestDefinition->ObjectiveClasses;
			const FString OwnerName = GetOwner() ? GetOwner()->GetName() : GetName();
			if (Objectives.Num() != ExpectedClasses.Num())
			{
				const FString Message = FString::Printf(
					TEXT("[%s] Objectives count (%d) does not match QuestDefinition '%s' (expects %d)."),
					*OwnerName, Objectives.Num(), *QuestDefinition->GetName(), ExpectedClasses.Num());

				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, Message);
				}
				return;
			}
			for (int32 Index = 0; Index < Objectives.Num(); ++Index)
			{
				const UQuestObjective *Objective = Objectives[Index];
				const TSubclassOf<UQuestObjective> ExpectedClass = ExpectedClasses[Index];

				if (!Objective || !ExpectedClass || Objective->GetClass() != ExpectedClass)
				{
					const FString FoundName = Objective ? Objective->GetClass()->GetName() : TEXT("None");
					const FString ExpectedName = ExpectedClass ? ExpectedClass->GetName() : TEXT("None");
					const FString Message = FString::Printf(
						TEXT("[%s] Objective[%d] mismatch - found '%s', expected '%s'."),
						*OwnerName, Index, *FoundName, *ExpectedName);

					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, Message);
					}
				}
			}
		}

		if (QuestDefinition->bOverridePrerequisit)
		{
			const TArray<TSubclassOf<UQuestPrerequisite>> &ExpectedClasses = QuestDefinition->PrerequisitClasses;
			const FString OwnerName = GetOwner() ? GetOwner()->GetName() : GetName();
			if (Prerequisites.Num() != ExpectedClasses.Num())
			{
				const FString Message = FString::Printf(
					TEXT("[%s] Prerequisites count (%d) does not match QuestDefinition '%s' (expects %d)."),
					*OwnerName, Prerequisites.Num(), *QuestDefinition->GetName(), ExpectedClasses.Num());

				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, Message);
				}
				return;
			}
			for (int32 Index = 0; Index < Prerequisites.Num(); ++Index)
			{
				const UQuestPrerequisite *Prerequisit = Prerequisites[Index];
				const TSubclassOf<UQuestPrerequisite> ExpectedClass = ExpectedClasses[Index];

				if (!Prerequisit || !ExpectedClass || Prerequisit->GetClass() != ExpectedClass)
				{
					const FString FoundName = Prerequisit ? Prerequisit->GetClass()->GetName() : TEXT("None");
					const FString ExpectedName = ExpectedClass ? ExpectedClass->GetName() : TEXT("None");
					const FString Message = FString::Printf(
						TEXT("[%s] Prerequisit[%d] mismatch - found '%s', expected '%s'."),
						*OwnerName, Index, *FoundName, *ExpectedName);

					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, Message);
					}
				}
			}
		}
	}
}
#endif
