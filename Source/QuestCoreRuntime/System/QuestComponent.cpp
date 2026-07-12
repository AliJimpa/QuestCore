#include "QuestComponent.h"
#include "Subsystem/QuestSubsystem.h"

bool UQuestComponent::ArePrerequisitesSatisfied() const
{
	for (const UQuestPrerequisite* Prerequisite : Prerequisites)
	{
		if (!Prerequisite || !Prerequisite->IsSatisfied())
		{
			return false;
		}
	}
	return true;
}

TArray<UQuestObjective*> UQuestComponent::GetCurrentGroupObjectives() const
{
	TArray<UQuestObjective*> Result;
	for (const FQuestObjectiveEntry& Entry : Objectives)
	{
		if (Entry.Order == CurrentOrder && Entry.Objective)
		{
			Result.Add(Entry.Objective);
		}
	}
	return Result;
}

void UQuestComponent::ActivateQuest()
{
	if (State == EQuestState::Active)
	{
		return;
	}

	if (!ArePrerequisitesSatisfied())
	{
		return;
	}

	State = EQuestState::Active;
	CurrentOrder = 0;
	BeginCurrentGroup();

	if (UQuestSubsystem* Subsystem = GetWorld() ? GetWorld()->GetSubsystem<UQuestSubsystem>() : nullptr)
	{
		Subsystem->NotifyQuestUpdated(this);
	}
}

void UQuestComponent::BeginCurrentGroup()
{
	for (UQuestObjective* Objective : GetCurrentGroupObjectives())
	{
		Objective->Begin();
	}
}

void UQuestComponent::EndCurrentGroup()
{
	for (UQuestObjective* Objective : GetCurrentGroupObjectives())
	{
		Objective->End();
	}
}

void UQuestComponent::UpdateQuest()
{
	if (State != EQuestState::Active)
	{
		return;
	}

	const TArray<UQuestObjective*> CurrentGroup = GetCurrentGroupObjectives();
	if (CurrentGroup.Num() == 0)
	{
		// No objectives at this order - nothing left to do, treat as complete.
		AdvanceOrFinish();
		return;
	}

	bool bAnyFailed = false;
	bool bAllDone = true;

	for (const UQuestObjective* Objective : CurrentGroup)
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

	if (bAnyFailed)
	{
		EndCurrentGroup();
		State = EQuestState::Failed;
		OnQuestFailed.Broadcast(this);
	}
	else if (bAllDone)
	{
		AdvanceOrFinish();
	}
	else
	{
		OnQuestUpdated.Broadcast(this);
	}

	if (UQuestSubsystem* Subsystem = GetWorld() ? GetWorld()->GetSubsystem<UQuestSubsystem>() : nullptr)
	{
		Subsystem->NotifyQuestUpdated(this);
	}
}

void UQuestComponent::AdvanceOrFinish()
{
	EndCurrentGroup();

	// Find the next distinct order greater than the current one.
	int32 NextOrder = INT32_MAX;
	for (const FQuestObjectiveEntry& Entry : Objectives)
	{
		if (Entry.Order > CurrentOrder && Entry.Order < NextOrder)
		{
			NextOrder = Entry.Order;
		}
	}

	if (NextOrder == INT32_MAX)
	{
		State = EQuestState::Completed;
		OnQuestCompleted.Broadcast(this);
		return;
	}

	CurrentOrder = NextOrder;
	BeginCurrentGroup();
	OnQuestUpdated.Broadcast(this);
}

float UQuestComponent::GetCurrentGroupProgress() const
{
	const TArray<UQuestObjective*> CurrentGroup = GetCurrentGroupObjectives();
	if (CurrentGroup.Num() == 0)
	{
		return 1.f;
	}

	float Total = 0.f;
	for (const UQuestObjective* Objective : CurrentGroup)
	{
		Total += Objective->GetProgress();
	}
	return Total / CurrentGroup.Num();
}
