#include "QuestObjective.h"
#include "QuestComponent.h"

class UWorld *UQuestObjective::GetWorld() const
{
#if WITH_EDITOR
    if (World == nullptr)
        LOG_ERROR("Objective's World Not Valid");
#endif
    return World;
}

void UQuestObjective::Construction(UQuestComponent *Quest)
{
    MyQuest = Quest;
    if (World == nullptr)
    {
        World = Quest->GetWorld();
    }
}

void UQuestObjective::UpdateQuest()
{
    if (MyQuest != nullptr)
    {
        MyQuest->UpdateQuest();
    }
}

UQuestComponent *UQuestObjective::GetQuestComponent() const
{
    return MyQuest;
}

AActor *UQuestObjective::GetOwner() const
{
    return MyQuest ? MyQuest->GetOwner() : nullptr;
}
