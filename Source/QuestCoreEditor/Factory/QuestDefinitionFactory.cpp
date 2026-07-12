#include "QuestDefinitionFactory.h"
#include "System/QuestDefinition.h"

UQuestDefinitionFactory::UQuestDefinitionFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UQuestDefinition::StaticClass();
}

UObject* UQuestDefinitionFactory::FactoryCreateNew(
	UClass* Class,
	UObject* InParent,
	FName Name,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn)
{
	return NewObject<UQuestDefinition>(InParent, Class, Name, Flags);
}
