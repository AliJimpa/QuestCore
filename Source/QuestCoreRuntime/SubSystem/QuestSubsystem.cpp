#include "QuestSubsystem.h"
#include "System/QuestComponent.h"
#include "Engine/QuestDebug.h"

void UQuestSubsystem::NotifyQuestUpdated(UQuestComponent *Quest)
{
	if (!Quest)
	{
		return;
	}

	switch (Quest->GetQuestState())
	{
	case EQuestState::Active:
		ActiveQuests.AddUnique(Quest);
		break;
	case EQuestState::Completed:
	case EQuestState::Failed:
		ActiveQuests.Remove(Quest);
		break;
	default:
		break;
	}
}

void UQuestSubsystem::RegisterQuest(UQuestComponent *Quest)
{
	if (Quest)
	{
		RegisteredQuests.AddUnique(Quest);
	}
	else
	{
		LOG_ERROR("Quest Not Valid for Register!");
	}
}

void UQuestSubsystem::UnregisterQuest(UQuestComponent *Quest)
{
	if (Quest)
	{
		RegisteredQuests.Remove(Quest);
		ActiveQuests.Remove(Quest);
	}
	else
	{
		LOG_ERROR("Quest Not Valid for Unregister!");
	}
}

bool UQuestSubsystem::ActivateQuest(UQuestDefinition *Definition)
{
	UQuestComponent *Quest = FindQuestByDefinition(Definition);
	if (!Quest || !Quest->ArePrerequisitesSatisfied())
	{
		return false;
	}

	Quest->ActivateQuest();
	return true;
}

bool UQuestSubsystem::ActivateQuestById(FName QuestId)
{
	UQuestComponent *Quest = FindQuestById(QuestId);
	if (!Quest || !Quest->ArePrerequisitesSatisfied())
	{
		return false;
	}

	Quest->ActivateQuest();
	return true;
}

TArray<UQuestComponent *> UQuestSubsystem::GetActiveQuests() const
{
	TArray<UQuestComponent *> Result;
	Result.Reserve(ActiveQuests.Num());
	for (UQuestComponent *Quest : ActiveQuests)
	{
		Result.Add(Quest);
	}
	return Result;
}

TArray<UQuestComponent *> UQuestSubsystem::GetAvailableQuests() const
{
	TArray<UQuestComponent *> Result;
	for (UQuestComponent *Quest : RegisteredQuests)
	{
		if (Quest && Quest->GetQuestState() == EQuestState::NotStarted && Quest->ArePrerequisitesSatisfied())
		{
			Result.Add(Quest);
		}
	}
	return Result;
}

UQuestComponent *UQuestSubsystem::FindQuestById(FName QuestId) const
{
	for (UQuestComponent *Quest : RegisteredQuests)
	{
		if (Quest && Quest->GetQuestId() == QuestId)
		{
			return Quest;
		}
	}
	return nullptr;
}

UQuestComponent *UQuestSubsystem::FindQuestByDefinition(UQuestDefinition *Definition) const
{
	for (UQuestComponent *Quest : RegisteredQuests)
	{
		if (Quest && Quest->QuestDefinition == Definition)
		{
			return Quest;
		}
	}
	return nullptr;
}
