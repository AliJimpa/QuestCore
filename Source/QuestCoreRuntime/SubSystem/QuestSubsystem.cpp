#include "QuestSubsystem.h"
#include "System/QuestComponent.h"

void UQuestSubsystem::RegisterQuest(UQuestComponent* Quest)
{
	if (Quest)
	{
		RegisteredQuests.AddUnique(Quest);
	}
}

void UQuestSubsystem::UnregisterQuest(UQuestComponent* Quest)
{
	RegisteredQuests.Remove(Quest);
	ActiveQuests.Remove(Quest);
}

TArray<UQuestComponent*> UQuestSubsystem::GetAvailableQuests() const
{
	TArray<UQuestComponent*> Result;
	for (UQuestComponent* Quest : RegisteredQuests)
	{
		if (Quest
			&& Quest->GetQuestState() == EQuestState::NotStarted
			&& Quest->ArePrerequisitesSatisfied())
		{
			Result.Add(Quest);
		}
	}
	return Result;
}

TArray<UQuestComponent*> UQuestSubsystem::GetActiveQuests() const
{
	TArray<UQuestComponent*> Result;
	Result.Reserve(ActiveQuests.Num());
	for (UQuestComponent* Quest : ActiveQuests)
	{
		Result.Add(Quest);
	}
	return Result;
}

UQuestComponent* UQuestSubsystem::FindQuestById(FName QuestId) const
{
	for (UQuestComponent* Quest : RegisteredQuests)
	{
		if (Quest && Quest->GetQuestId() == QuestId)
		{
			return Quest;
		}
	}
	return nullptr;
}

UQuestComponent* UQuestSubsystem::FindQuestByDefinition(UQuestDefinition* Definition) const
{
	for (UQuestComponent* Quest : RegisteredQuests)
	{
		if (Quest && Quest->QuestDefinition == Definition)
		{
			return Quest;
		}
	}
	return nullptr;
}

bool UQuestSubsystem::ActivateQuest(FName QuestId)
{
	UQuestComponent* Quest = FindQuestById(QuestId);
	if (!Quest || !Quest->ArePrerequisitesSatisfied())
	{
		return false;
	}

	Quest->ActivateQuest();
	return true;
}

void UQuestSubsystem::NotifyQuestUpdated(UQuestComponent* Quest)
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
