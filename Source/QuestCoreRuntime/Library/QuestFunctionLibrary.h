#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QuestFunctionLibrary.generated.h"

class UQuestComponent;
class UQuestDefinition;

/**
 * Static entry points for activating/deactivating quests by QuestId.
 * Moved out of UQuestSubsystem so quest control doesn't require holding
 * a subsystem reference - callable from anywhere with a world context
 * (Blueprint graphs, gameplay code, etc).
 */
UCLASS()
class QUESTCORERUNTIME_API UQuestFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Activates the quest matching the given definition asset.
	 * @param WorldContextObject Any object in the target world (used to resolve the QuestSubsystem).
	 * @param Definition The quest's data asset - resolved to a registered UQuestComponent internally.
	 * @return true if a matching quest was found and activation was attempted; false if no quest is registered for this definition.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest", meta = (WorldContext = "WorldContextObject"))
	static bool ActivateQuest(const UObject *WorldContextObject, UQuestDefinition *Definition);
	/**
	 * Activates the quest matching the given QuestId.
	 * @param WorldContextObject Any object in the target world (used to resolve the QuestSubsystem).
	 * @param QuestId The QuestId to look up (see UQuestDefinition::QuestId).
	 * @return true if a matching quest was found and activation was attempted; false if no quest is registered under this id.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest", meta = (WorldContext = "WorldContextObject"))
	static bool ActivateQuestById(const UObject *WorldContextObject, FName QuestId);
	/**
	 * Deactivates the quest matching the given definition asset.
	 * @param WorldContextObject Any object in the target world (used to resolve the QuestSubsystem).
	 * @param Definition The quest's data asset - resolved to a registered UQuestComponent internally.
	 * @return true if a matching quest was found and deactivation was attempted; false if no quest is registered for this definition.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest", meta = (WorldContext = "WorldContextObject"))
	static bool DeactivateQuest(const UObject *WorldContextObject, UQuestDefinition *Definition);
	/**
	 * Deactivates the quest matching the given QuestId.
	 * @param WorldContextObject Any object in the target world (used to resolve the QuestSubsystem).
	 * @param QuestId The QuestId to look up (see UQuestDefinition::QuestId).
	 * @return true if a matching quest was found and deactivation was attempted; false if no quest is registered under this id.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest", meta = (WorldContext = "WorldContextObject"))
	static bool DeactivateQuestById(const UObject *WorldContextObject, FName QuestId);

	/**
	 * Returns every quest currently in the Active state.
	 * @return All registered quests whose state is EQuestState::Active.
	 */
	UFUNCTION(BlueprintPure, Category = "Quest", meta = (WorldContext = "WorldContextObject"))
	static TArray<UQuestComponent *> GetActiveQuests(const UObject *WorldContextObject);
	/**
	 * Returns every quest that could be activated right now.
	 * @return All registered quests still NotStarted whose prerequisites are currently satisfied.
	 */
	UFUNCTION(BlueprintPure, Category = "Quest", meta = (WorldContext = "WorldContextObject"))
	static TArray<UQuestComponent *> GetAvailableQuests(const UObject *WorldContextObject);
	/**
	 * Looks up a registered quest by its QuestId.
	 * @param QuestId The QuestId to search for (see UQuestDefinition::QuestId).
	 * @return The matching quest, or nullptr if no registered quest has this id.
	 */
	UFUNCTION(BlueprintPure, Category = "Quest", meta = (WorldContext = "WorldContextObject"))
	static UQuestComponent *FindQuestById(const UObject *WorldContextObject, FName QuestId);
	/**
	 * Looks up a registered quest by its definition asset.
	 * @param Definition The quest definition asset to search for.
	 * @return The matching quest, or nullptr if no registered quest uses this definition.
	 */
	UFUNCTION(BlueprintPure, Category = "Quest", meta = (WorldContext = "WorldContextObject"))
	static UQuestComponent *FindQuestByDefinition(const UObject *WorldContextObject, UQuestDefinition *Definition);

	/**
	 * Checks whether the quest matching QuestId has completed.
	 * @param WorldContextObject Any object in the target world (used to resolve the QuestSubsystem).
	 * @param QuestId The QuestId to look up (see UQuestDefinition::QuestId).
	 * @return true if a matching quest is registered and its state is Completed; false otherwise (including if not found).
	 */
	UFUNCTION(BlueprintPure, Category = "Quest", meta = (WorldContext = "WorldContextObject"))
	static bool IsQuestCompletedById(const UObject *WorldContextObject, FName QuestId);

	/**
	 * Checks whether the quest matching Definition has completed.
	 * @param WorldContextObject Any object in the target world (used to resolve the QuestSubsystem).
	 * @param Definition The quest definition asset to look up.
	 * @return true if a matching quest is registered and its state is Completed; false otherwise (including if not found).
	 */
	UFUNCTION(BlueprintPure, Category = "Quest", meta = (WorldContext = "WorldContextObject"))
	static bool IsQuestCompletedByDefinition(const UObject *WorldContextObject, UQuestDefinition *Definition);

	/**
	 * Checks whether the quest matching QuestId has failed.
	 * @param WorldContextObject Any object in the target world (used to resolve the QuestSubsystem).
	 * @param QuestId The QuestId to look up (see UQuestDefinition::QuestId).
	 * @return true if a matching quest is registered and its state is Failed; false otherwise (including if not found).
	 */
	UFUNCTION(BlueprintPure, Category = "Quest", meta = (WorldContext = "WorldContextObject"))
	static bool IsQuestFailedById(const UObject *WorldContextObject, FName QuestId);

	/**
	 * Checks whether the quest matching Definition has failed.
	 * @param WorldContextObject Any object in the target world (used to resolve the QuestSubsystem).
	 * @param Definition The quest definition asset to look up.
	 * @return true if a matching quest is registered and its state is Failed; false otherwise (including if not found).
	 */
	UFUNCTION(BlueprintPure, Category = "Quest", meta = (WorldContext = "WorldContextObject"))
	static bool IsQuestFailedByDefinition(const UObject *WorldContextObject, UQuestDefinition *Definition);
};
