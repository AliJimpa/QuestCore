#include "QuestSubsystem.h"
#include "System/QuestComponent.h"
#include "Engine/QuestDebug.h"
#include "Engine/QuestSaveGame.h"

void UQuestSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
	Super::Initialize(Collection);
	LoadQuestData();
}

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
	LOG("Quest [%s] Registerd", *QuestId.ToString());

	if (!QuestId.IsNone())
	{
		if (const EQuestState *SavedState = PendingLoadedStates.Find(QuestId))
		{
			Quest->ApplyLoadedState(*SavedState);
		}
	}

	OnAnyQuestRegister.Broadcast(Quest);
	return true;
}
void UQuestSubsystem::SubmitQuestActivation(UQuestComponent *Quest, bool bIsStarted)
{
	const FName QuestId = Quest->GetQuestId();
	if (bIsStarted)
	{
		ActiveQuests.AddUnique(Quest);
		LOG("Quest [%s] Started", *QuestId.ToString());
	}
	else
	{
		ActiveQuests.Remove(Quest);
		LOG("Quest [%s] Reset", *QuestId.ToString());
	}
}
bool UQuestSubsystem::UnregisterQuest(UQuestComponent *Quest)
{
	if (Quest)
	{
		RegisteredQuests.Remove(Quest);
		ActiveQuests.Remove(Quest);
		const FName QuestId = Quest->GetQuestId();
		LOG("Quest [%s] Unregisterd", *QuestId.ToString());
		OnAnyQuestUnregister.Broadcast(Quest);
		return true;
	}
	else
	{
		LOG_ERROR("Quest Not Valid for Unregister!");
		return false;
	}
}

void UQuestSubsystem::NotifyQuestUpdated(UQuestComponent *Quest, bool bIsStarted)
{
	if (Quest != nullptr)
	{
		if (bIsStarted)
		{
			OnAnyQuestStarted.Broadcast(Quest);
		}
		else
		{
			OnAnyQuestReset.Broadcast(Quest);
		}
	}
	else
	{
		LOG_ERROR("Quest Not Valid for Notify!");
	}
}

void UQuestSubsystem::LoadQuestData()
{
	PendingLoadedStates.Empty();

	const UQuestCoreSettings *Settings = GetDefault<UQuestCoreSettings>();

	if (!UGameplayStatics::DoesSaveGameExist(Settings->SaveSlotName, Settings->SaveUserIndex))
	{
		return;
	}

	// Cast<UQuestSaveGame> still works even if the actual loaded object
	// is a project-specific subclass - we only need the base fields here.
	if (UQuestSaveGame *SaveGame = Cast<UQuestSaveGame>(UGameplayStatics::LoadGameFromSlot(Settings->SaveSlotName, Settings->SaveUserIndex)))
	{
		PendingLoadedStates = SaveGame->QuestStates;
	}
	else
	{
		LOG_ERROR("Quest save file exists but failed to load/cast.");
	}
}
void UQuestSubsystem::SaveQuestData()
{
	const UQuestCoreSettings *Settings = GetDefault<UQuestCoreSettings>();

	// Guard against a bad/unset config value rather than crashing
	// CreateSaveGameObject with a null class.
	TSubclassOf<UQuestSaveGame> ClassToUse = Settings->SaveGameClass;
	if (!ClassToUse)
	{
		ClassToUse = UQuestSaveGame::StaticClass();
	}

	UQuestSaveGame *SaveGame = Cast<UQuestSaveGame>(UGameplayStatics::CreateSaveGameObject(ClassToUse));
	if (!SaveGame)
	{
		LOG_ERROR("Failed to create save game instance of class '%s'.", *ClassToUse->GetName());
		return;
	}

	for (const UQuestComponent *Quest : RegisteredQuests)
	{
		if (!Quest)
		{
			continue;
		}
		const FName QuestId = Quest->GetQuestId();
		if (!QuestId.IsNone())
		{
			SaveGame->QuestStates.Add(QuestId, Quest->GetQuestState());
		}
	}

	UGameplayStatics::SaveGameToSlot(SaveGame, Settings->SaveSlotName, Settings->SaveUserIndex);
}

bool UQuestSubsystem::StartQuestById(FName QuestId)
{
	UQuestComponent *Quest = FindQuestById(QuestId);
	if (!Quest)
	{
		return false;
	}

	return Quest->StartQuest();
}
bool UQuestSubsystem::ResetQuestById(FName QuestId)
{
	UQuestComponent *Quest = FindQuestById(QuestId);
	if (!Quest)
	{
		return false;
	}

	return Quest->ResetQuest();
}
bool UQuestSubsystem::StartQuest(const UQuestDefinition *Definition)
{
	UQuestComponent *Quest = FindQuestByDefinition(Definition);
	if (!Quest)
	{
		return false;
	}

	return Quest->StartQuest();
}
bool UQuestSubsystem::ResetQuest(const UQuestDefinition *Definition)
{
	UQuestComponent *Quest = FindQuestByDefinition(Definition);
	if (!Quest)
	{
		return false;
	}

	return Quest->ResetQuest();
}

TArray<UQuestComponent *> UQuestSubsystem::GetStartedQuests() const
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
UQuestComponent *UQuestSubsystem::FindQuestByDefinition(const UQuestDefinition *Definition) const
{
	for (UQuestComponent *Quest : RegisteredQuests)
	{
		if (Quest && Quest->GetDefinition() == Definition)
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
bool UQuestSubsystem::IsQuestCompletedByDefinition(const UQuestDefinition *Definition) const
{
	const UQuestComponent *Quest = FindQuestByDefinition(Definition);
	return Quest && Quest->IsQuestCompleted();
}
bool UQuestSubsystem::IsQuestFailedById(FName QuestId) const
{
	const UQuestComponent *Quest = FindQuestById(QuestId);
	return Quest && Quest->IsQuestFailed();
}
bool UQuestSubsystem::IsQuestFailedByDefinition(const UQuestDefinition *Definition) const
{
	const UQuestComponent *Quest = FindQuestByDefinition(Definition);
	return Quest && Quest->IsQuestFailed();
}
