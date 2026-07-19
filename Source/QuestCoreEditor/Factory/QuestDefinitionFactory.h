#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "System/QuestDefinition.h"
#include "QuestDefinitionFactory.generated.h"

UCLASS()
class QUESTCOREEDITOR_API UQuestDefinitionFactory : public UFactory
{
	GENERATED_BODY()

public:
	UQuestDefinitionFactory()
	{
		bCreateNew = true;
		bEditAfterNew = true;
		SupportedClass = UQuestDefinition::StaticClass();
	}

	virtual UObject *FactoryCreateNew(UClass *Class, UObject *InParent, FName Name, EObjectFlags Flags, UObject *Context, FFeedbackContext *Warn) override
	{
		return NewObject<UQuestDefinition>(InParent, Class, Name, Flags);
	}
};
