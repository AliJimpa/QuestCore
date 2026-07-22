#include "QuestObjective.h"
#include "QuestComponent.h"

class UWorld *UQuestObjective::GetWorld() const
{
    return MyQuest ? MyQuest->GetWorld() : nullptr;
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
