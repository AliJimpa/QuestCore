#include "QuestSubsystem.h"
#include "System/QuestComponent.h"
#include "Engine/QuestDebug.h"

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
void UQuestSubsystem::SubmitQuestActivation(UQuestComponent *Quest, bool bIsActive)
{
	if (bIsActive)
	{
		ActiveQuests.AddUnique(Quest);
	}
	else
	{
		ActiveQuests.Remove(Quest);
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

bool UQuestSubsystem::IsQuestCompletedById(FName QuestId) const
{
	const UQuestComponent *Quest = FindQuestById(QuestId);
	return Quest && Quest->IsQuestCompleted();
}

bool UQuestSubsystem::IsQuestCompletedByDefinition(UQuestDefinition *Definition) const
{
	const UQuestComponent *Quest = FindQuestByDefinition(Definition);
	return Quest && Quest->IsQuestCompleted();
}

bool UQuestSubsystem::IsQuestFailedById(FName QuestId) const
{
	const UQuestComponent *Quest = FindQuestById(QuestId);
	return Quest && Quest->IsQuestFailed();
}

bool UQuestSubsystem::IsQuestFailedByDefinition(UQuestDefinition *Definition) const
{
	const UQuestComponent *Quest = FindQuestByDefinition(Definition);
	return Quest && Quest->IsQuestFailed();
}
