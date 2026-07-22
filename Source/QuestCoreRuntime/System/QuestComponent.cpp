#include "QuestComponent.h"
#include "Engine/QuestDebug.h"
#include "QuestPrerequisite.h"
#include "QuestObjective.h"
#include "QuestEvent.h"
#include "SubSystem/QuestSubsystem.h"

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

	if (QuestDefinition != nullptr)
	{
		for (UQuestObjective *Objective : QuestDefinition->Objectives)
		{
			Objective->Construction(this);
		}
	}
	else
	{
		LOG_ERROR("[%s]: QuestDefinition is not valid", *GetOwner()->GetName());
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
	for (UQuestObjective *Objective : QuestDefinition->Objectives)
	{
		Objective->Begin();
	}
}
void UQuestComponent::SetState(const EQuestState NewState)
{
	const EQuestState LastState = State;
	State = NewState;
	GetQuestSubsystem()->NotifyQuestUpdated(this);
	switch (State)
	{
	case EQuestState::NotStarted:
		if (LastState == EQuestState::InProgress)
			EndObjectives();
		break;
	case EQuestState::InProgress:
		BeginObjectives();
		break;
	case EQuestState::Completed:
		EndObjectives();
		OnQuestCompleted.Broadcast(this);
		InvokeQuestEvents(true);
		if (bAutoSave)
		{
			GetQuestSubsystem()->SaveQuestData();
		}
		if (bAutoDestroy)
		{
			GetOwner()->Destroy();
		}
		break;
	case EQuestState::Failed:
		EndObjectives();
		OnQuestFailed.Broadcast(this);
		InvokeQuestEvents(false);
		if (bAutoSave)
		{
			GetQuestSubsystem()->SaveQuestData();
		}
		if (bAutoDestroy)
		{
			GetOwner()->Destroy();
		}
		break;
	}
}
void UQuestComponent::InvokeQuestEvents(bool IsCompleted)
{
	if (QuestDefinition != nullptr)
	{
		if (IsCompleted)
		{
			for (UQuestEvent *Event : QuestDefinition->OnCompleted)
			{
				Event->Execute(this);
			}
		}
		else
		{
			for (UQuestEvent *Event : QuestDefinition->OnFailed)
			{
				Event->Execute(this);
			}
		}
	}
}
void UQuestComponent::EndObjectives()
{
	for (UQuestObjective *Objective : QuestDefinition->Objectives)
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
		LOG("[%s]: Quest already is Deactive.", *GetOwner()->GetName());
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
	for (const UQuestObjective *Objective : QuestDefinition->Objectives)
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

bool UQuestComponent::ArePrerequisitesSatisfied() const
{
	for (const UQuestPrerequisite *Prerequisite : QuestDefinition->Prerequisites)
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
	return QuestDefinition ? QuestDefinition->QuestId : NAME_None;
}
float UQuestComponent::GetProgress() const
{
	float Total = 0.f;
	for (const UQuestObjective *Objective : QuestDefinition->Objectives)
	{
		Total += Objective->GetProgress();
	}
	return Total / QuestDefinition->Objectives.Num();
}
int32 UQuestComponent::GetMaxObjective() const
{
	return QuestDefinition != nullptr ? QuestDefinition->Objectives.Num() : -1;
}
int32 UQuestComponent::GetCurrentObjectiveIndex() const
{
	int32 Reesult = 0;
	for (const UQuestObjective *Objective : QuestDefinition->Objectives)
	{
		if (Objective && Objective->GetState() == EQuestObjectiveState::InProgress)
		{
			return Reesult;
		}
		Reesult++;
	}
	return -1;
}
UQuestObjective *UQuestComponent::GetObjective(const int32 Index) const
{
	if (QuestDefinition->Objectives.Num() - 1 >= Index)
	{
		if (UQuestObjective *Objective = QuestDefinition->Objectives[Index])
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
	if (QuestDefinition)
	{
		for (UQuestPrerequisite *Prerequisite : QuestDefinition->Prerequisites)
		{
			Prerequisite->OnVisualize(this);
		}
		for (UQuestObjective *Objective : QuestDefinition->Objectives)
		{
			Objective->OnVisualize(this);
		}
		for (UQuestEvent *Event : QuestDefinition->OnCompleted)
		{
			Event->OnVisualize(this);
		}
		for (UQuestEvent *Event : QuestDefinition->OnFailed)
		{
			Event->OnVisualize(this);
		}
	}
}
#endif
