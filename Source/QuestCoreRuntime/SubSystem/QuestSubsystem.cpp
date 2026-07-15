#include "QuestSubsystem.h"
#include "System/QuestComponent.h"
#include "Engine/QuestDebug.h"

bool UQuestSubsystem::RegisterQuest(UQuestComponent *Quest)
{
	if (!Quest)
	{
		LOG_ERROR("Quest Not Valid for Register!");
		return false;
	}

	const FName QuestId = Quest->GetQuestId();
	if (UQuestComponent *Existing = FindQuestById(QuestId))
	{
		const FString Message = FString::Printf(
			TEXT("Quest registration failed: QuestId '%s' is already registered (by '%s')."),
			*QuestId.ToString(),
			Existing->GetOwner() ? *Existing->GetOwner()->GetName() : TEXT("Unknown"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, Message);
		}
		LOG_ERROR("%s", *Message);
		return false;
	}

	RegisteredQuests.AddUnique(Quest);
	return true;
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
bool UQuestSubsystem::UnregisterQuest(UQuestComponent *Quest)
{
	if (Quest)
	{
		RegisteredQuests.Remove(Quest);
		ActiveQuests.Remove(Quest);
		return true;
	}
	else
	{
		LOG_ERROR("Quest Not Valid for Unregister!");
		return false;
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
