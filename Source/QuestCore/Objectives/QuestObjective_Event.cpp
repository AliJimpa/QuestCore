#include "QuestObjective_Event.h"

void UQuestObjective_Event::NotifyDone()
{
	CachedState = EQuestObjectiveState::Done;
}

void UQuestObjective_Event::NotifyFailed()
{
	CachedState = EQuestObjectiveState::Failed;
}
