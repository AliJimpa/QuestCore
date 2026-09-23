#include "QuestComponent.h"
#include "Engine/QuestDebug.h"
#include "QuestPrerequisite.h"
#include "QuestObjective.h"
#include "QuestEvent.h"
#include "SubSystem/QuestSubsystem.h"
#include "QuestDefinition.h"

void UQuestComponent::ApplyLoadedState(EQuestState SavedState)
{
	switch (SavedState)
	{
	case EQuestState::InProgress:
		SetState(EQuestState::InProgress);
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
			LOG_ERROR("[%s]: Can't find UQuestSubsystem for Register", *GetOwner()->GetName());
			return nullptr;
		}
	}

	return CachedSubsystem.Get();
}

void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	RuntimeQuestDefinition = DuplicateObject<UQuestDefinition>(QuestDefinition, this);
	if (RuntimeQuestDefinition != nullptr)
	{
		for (UQuestObjective *Objective : RuntimeQuestDefinition->Objectives)
		{
			Objective->Construction(this);
		}
	}
	else
	{
		LOG_ERROR("[%s]: RuntimeQuestDefinition  is not valid", *GetOwner()->GetName());
		return;
	}

	const bool IsRegisterd = GetQuestSubsystem()->RegisterQuest(this);
	if (!IsRegisterd)
		return;

	if (bAutoActive && State == EQuestState::NotStarted)
	{
		StartQuest();
	}
}
void UQuestComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (State == EQuestState::InProgress)
		ResetQuest();

	GetQuestSubsystem()->UnregisterQuest(this);

	Super::EndPlay(EndPlayReason);
}

void UQuestComponent::BeginObjectives()
{
	for (UQuestObjective *Objective : RuntimeQuestDefinition->Objectives)
	{
		Objective->Begin();
	}
}
void UQuestComponent::SetState(const EQuestState NewState)
{
	const EQuestState LastState = State;
	State = NewState;
	switch (State)
	{
	case EQuestState::NotStarted:
		if (LastState == EQuestState::InProgress)
		{
			EndObjectives();
			GetQuestSubsystem()->NotifyQuestUpdated(this, false);
		}
		break;
	case EQuestState::InProgress:
		BeginObjectives();
		GetQuestSubsystem()->NotifyQuestUpdated(this, true);
		break;
	case EQuestState::Completed:
		EndObjectives();
		OnQuestUpdated.Broadcast(this);
		OnQuestCompleted.Broadcast(this);
		InvokeQuestEvents(true);
		if (bAutoSave)
		{
			GetQuestSubsystem()->SaveQuestData();
		}
		GetQuestSubsystem()->NotifyQuestUpdated(this, false);
		if (bAutoDestroy)
		{
			GetOwner()->Destroy();
		}
		break;
	case EQuestState::Failed:
		EndObjectives();
		OnQuestUpdated.Broadcast(this);
		OnQuestFailed.Broadcast(this);
		InvokeQuestEvents(false);
		if (bAutoSave)
		{
			GetQuestSubsystem()->SaveQuestData();
		}
		GetQuestSubsystem()->NotifyQuestUpdated(this, false);
		if (bAutoDestroy)
		{
			GetOwner()->Destroy();
		}
		break;
	}
}
void UQuestComponent::InvokeQuestEvents(bool IsCompleted)
{
	if (RuntimeQuestDefinition != nullptr)
	{
		if (IsCompleted)
		{
			for (UQuestEvent *Event : RuntimeQuestDefinition->OnCompleted)
			{
				Event->Execute(this);
			}
		}
		else
		{
			for (UQuestEvent *Event : RuntimeQuestDefinition->OnFailed)
			{
				Event->Execute(this);
			}
		}
	}
}
void UQuestComponent::EndObjectives()
{
	for (UQuestObjective *Objective : RuntimeQuestDefinition->Objectives)
	{
		Objective->End();
	}
}

bool UQuestComponent::StartQuest()
{
	if (State == EQuestState::InProgress)
	{
		LOG_WARNING("[%s]: Quest already is InProgress.", *GetOwner()->GetName());
		return false;
	}

	if (!ArePrerequisitesSatisfied())
	{
		LOG_WARNING("[%s]: Prerequisites not Satisfied.", *GetOwner()->GetName());
		return false;
	}

	SetState(EQuestState::InProgress);
	GetQuestSubsystem()->SubmitQuestActivation(this, true);

	return true;
}
bool UQuestComponent::ResetQuest()
{
	if (State == EQuestState::NotStarted)
	{
		LOG("[%s]: Quest already is NotStarted.", *GetOwner()->GetName());
		return false;
	}

	SetState(EQuestState::NotStarted);
	GetQuestSubsystem()->SubmitQuestActivation(this, false);

	return true;
}
void UQuestComponent::UpdateQuest()
{
	if (State != EQuestState::InProgress)
	{
		return;
	}

	bool bAnyFailed = false;
	bool bAllDone = true;
	for (const UQuestObjective *Objective : RuntimeQuestDefinition->Objectives)
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
		SetState(EQuestState::Failed);
	}
	else
	{
		if (bAllDone)
		{
			SetState(EQuestState::Completed);
		}
		else
		{
			OnQuestUpdated.Broadcast(this);
		}
	}
}

bool UQuestComponent::ArePrerequisitesSatisfied() const
{
	for (const UQuestPrerequisite *Prerequisite : RuntimeQuestDefinition->Prerequisites)
	{
		if (!Prerequisite || !Prerequisite->IsSatisfied(this))
		{
			return false;
		}
	}
	return true;
}

FName UQuestComponent::GetQuestId() const
{
	return RuntimeQuestDefinition ? RuntimeQuestDefinition->QuestId : NAME_None;
}
float UQuestComponent::GetProgress() const
{
	float Total = 0.f;
	for (const UQuestObjective *Objective : RuntimeQuestDefinition->Objectives)
	{
		Total += Objective->GetProgress();
	}
	return Total / RuntimeQuestDefinition->Objectives.Num();
}
int32 UQuestComponent::GetMaxObjective() const
{
	return RuntimeQuestDefinition != nullptr ? RuntimeQuestDefinition->Objectives.Num() : -1;
}
int32 UQuestComponent::GetCurrentObjectiveIndex() const
{
	int32 Reesult = 0;
	for (const UQuestObjective *Objective : RuntimeQuestDefinition->Objectives)
	{
		if (Objective && Objective->GetState() == EQuestObjectiveState::InProgress)
		{
			return Reesult;
		}
		Reesult++;
	}
	return Reesult;
}
UQuestObjective *UQuestComponent::GetObjective(const int32 Index) const
{
	if (RuntimeQuestDefinition->Objectives.Num() - 1 >= Index)
	{
		if (UQuestObjective *Objective = RuntimeQuestDefinition->Objectives[Index])
		{
			return Objective;
		}
		else
		{
			LOG_WARNING("Objective With Index [%d] Is Not Valid", Index);
		}
	}
	else
	{
		LOG_WARNING("Can't find Objective With Index [%d]", Index);
	}
	return nullptr;
}

#if WITH_EDITOR
void UQuestComponent::Visualize()
{
	if (RuntimeQuestDefinition)
	{
		for (UQuestPrerequisite *Prerequisite : RuntimeQuestDefinition->Prerequisites)
		{
			Prerequisite->OnVisualize(this);
		}
		for (UQuestObjective *Objective : RuntimeQuestDefinition->Objectives)
		{
			Objective->OnVisualize(this);
		}
		for (UQuestEvent *Event : RuntimeQuestDefinition->OnCompleted)
		{
			Event->OnVisualize(this);
		}
		for (UQuestEvent *Event : RuntimeQuestDefinition->OnFailed)
		{
			Event->OnVisualize(this);
		}
	}
}
#endif
