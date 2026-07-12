#include "QuestComponent.h"
#include "SubSystem/QuestSubsystem.h"
#include "Engine/QuestDebug.h"

void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UQuestSubsystem *Subsystem = GetWorld() ? GetWorld()->GetSubsystem<UQuestSubsystem>() : nullptr)
	{
		Subsystem->RegisterQuest(this);
	}
	else
	{
		LOG_ERROR("Can't find UQuestSubsystem for Register");
	}

	if (bAutoActive)
	{
		ActivateQuest();
	}
}

void UQuestComponent::BeginObjectives()
{
	for (UQuestObjective *Objective : Objectives)
	{
		Objective->Begin(GetOwner(), QuestDefinition);
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
		break;
	case EQuestState::Failed:
		EndObjectives();
		OnQuestFailed.Broadcast(this);
		break;
	}
}

void UQuestComponent::ActivateQuest()
{
	if (State == EQuestState::Active)
	{
		LOG("Quest already is Active.");
		return;
	}

	if (!ArePrerequisitesSatisfied())
	{
		LOG_WARNING("Prerequisites not Satisfied.");
		return;
	}

	SetState(EQuestState::Active);

	if (UQuestSubsystem *Subsystem = GetWorld() ? GetWorld()->GetSubsystem<UQuestSubsystem>() : nullptr)
	{
		Subsystem->SubmitQuestActivation(this, true);
	}
	else
	{
		LOG_ERROR("Can't find UQuestSubsystem for Register");
	}
}

void UQuestComponent::DeactivateQuest()
{
	if (State == EQuestState::NotStarted)
	{
		LOG("Quest already is Deactive.");
		return;
	}

	SetState(EQuestState::NotStarted);

	if (UQuestSubsystem *Subsystem = GetWorld() ? GetWorld()->GetSubsystem<UQuestSubsystem>() : nullptr)
	{
		Subsystem->SubmitQuestActivation(this, false);
	}
	else
	{
		LOG_ERROR("Can't find UQuestSubsystem for Register");
	}
}

bool UQuestComponent::ArePrerequisitesSatisfied() const
{
	for (const UQuestPrerequisite *Prerequisite : Prerequisites)
	{
		if (!Prerequisite || !Prerequisite->IsSatisfied())
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

void UQuestComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DeactivateQuest();

	if (UQuestSubsystem *Subsystem = GetWorld() ? GetWorld()->GetSubsystem<UQuestSubsystem>() : nullptr)
	{
		Subsystem->UnregisterQuest(this);
	}
	else
	{
		LOG_ERROR("Can't find UQuestSubsystem for Register");
	}

	Super::EndPlay(EndPlayReason);
}
