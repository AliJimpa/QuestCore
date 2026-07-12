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
		LOG_ERROR("Can't find UQuestSubsystem for Register");
		return false;
	}

	UQuestComponent *Quest = Subsystem->FindQuestById(QuestId);
	if (!Quest)
	{
		return false;
	}

	Quest->ActivateQuest();
	return true;
}

bool UQuestFunctionLibrary::DeactivateQuestById(const UObject *WorldContextObject, FName QuestId)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for Register");
		return false;
	}

	UQuestComponent *Quest = Subsystem->FindQuestById(QuestId);
	if (!Quest)
	{
		return false;
	}

	Quest->DeactivateQuest();
	return true;
}

bool UQuestFunctionLibrary::ActivateQuest(const UObject *WorldContextObject, UQuestDefinition *Definition)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for Register");
		return false;
	}

	UQuestComponent *Quest = Subsystem->FindQuestByDefinition(Definition);
	if (!Quest)
	{
		return false;
	}

	Quest->ActivateQuest();
	return true;
}

bool UQuestFunctionLibrary::DeactivateQuest(const UObject *WorldContextObject, UQuestDefinition *Definition)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for Register");
		return false;
	}

	UQuestComponent *Quest = Subsystem->FindQuestByDefinition(Definition);
	if (!Quest)
	{
		return false;
	}

	Quest->DeactivateQuest();
	return true;
}

TArray<UQuestComponent *> UQuestFunctionLibrary::GetActiveQuests(const UObject *WorldContextObject)
{
	UWorld *World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	UQuestSubsystem *Subsystem = World ? World->GetSubsystem<UQuestSubsystem>() : nullptr;
	if (!Subsystem)
	{
		LOG_ERROR("Can't find UQuestSubsystem for Register");
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
		LOG_ERROR("Can't find UQuestSubsystem for Register");
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
		LOG_ERROR("Can't find UQuestSubsystem for Register");
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
		LOG_ERROR("Can't find UQuestSubsystem for Register");
		return nullptr;
	}
	return Subsystem->FindQuestByDefinition(Definition);
}
