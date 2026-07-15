#include "QuestFunctionLibrary.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "SubSystem/QuestSubsystem.h"
#include "System/QuestComponent.h"
#include "Engine/QuestDebug.h"

bool UQuestFunctionLibrary::ActivateQuestById(const UObject *WorldContextObject, FName QuestId)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for ActiveQuest");
		return false;
	}

	UQuestComponent *Quest = Subsystem->FindQuestById(QuestId);
	if (!Quest)
	{
		return false;
	}

	return Quest->ActivateQuest();
}

bool UQuestFunctionLibrary::DeactivateQuestById(const UObject *WorldContextObject, FName QuestId)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for Deactive");
		return false;
	}

	UQuestComponent *Quest = Subsystem->FindQuestById(QuestId);
	if (!Quest)
	{
		return false;
	}

	return Quest->DeactivateQuest();
}

bool UQuestFunctionLibrary::ActivateQuest(const UObject *WorldContextObject, UQuestDefinition *Definition)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for ActiveQuest");
		return false;
	}

	UQuestComponent *Quest = Subsystem->FindQuestByDefinition(Definition);
	if (!Quest)
	{
		return false;
	}

	return Quest->ActivateQuest();
}

bool UQuestFunctionLibrary::DeactivateQuest(const UObject *WorldContextObject, UQuestDefinition *Definition)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for Deactive");
		return false;
	}

	UQuestComponent *Quest = Subsystem->FindQuestByDefinition(Definition);
	if (!Quest)
	{
		return false;
	}

	return Quest->DeactivateQuest();
}

TArray<UQuestComponent *> UQuestFunctionLibrary::GetActiveQuests(const UObject *WorldContextObject)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for GetActiveQuests");
		return TArray<UQuestComponent *>();
	}

	return Subsystem->GetActiveQuests();
}

TArray<UQuestComponent *> UQuestFunctionLibrary::GetAvailableQuests(const UObject *WorldContextObject)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for GetAvalibale");
		return TArray<UQuestComponent *>();
	}
	return Subsystem->GetAvailableQuests();
}

UQuestComponent *UQuestFunctionLibrary::FindQuestById(const UObject *WorldContextObject, FName QuestId)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for Find");
		return nullptr;
	}
	return Subsystem->FindQuestById(QuestId);
}

UQuestComponent *UQuestFunctionLibrary::FindQuestByDefinition(const UObject *WorldContextObject, UQuestDefinition *Definition)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for Find");
		return nullptr;
	}
	return Subsystem->FindQuestByDefinition(Definition);
}

bool UQuestFunctionLibrary::IsQuestCompletedById(const UObject *WorldContextObject, FName QuestId)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for CheckCompleted");
		return false;
	}
	return Subsystem->IsQuestCompletedById(QuestId);
}
bool UQuestFunctionLibrary::IsQuestCompletedByDefinition(const UObject *WorldContextObject, UQuestDefinition *Definition)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for CheckCompleted");
		return false;
	}
	return Subsystem->IsQuestCompletedByDefinition(Definition);
}
bool UQuestFunctionLibrary::IsQuestFailedById(const UObject *WorldContextObject, FName QuestId)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for CheckFailed");
		return false;
	}
	return Subsystem->IsQuestFailedById(QuestId);
}
bool UQuestFunctionLibrary::IsQuestFailedByDefinition(const UObject *WorldContextObject, UQuestDefinition *Definition)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for CheckFailed");
		return false;
	}
	return Subsystem->IsQuestFailedByDefinition(Definition);
}

void UQuestFunctionLibrary::SaveAllQuest(const UObject *WorldContextObject)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for CheckCompleted");
		return;
	}

	Subsystem->SaveQuestData();
}
