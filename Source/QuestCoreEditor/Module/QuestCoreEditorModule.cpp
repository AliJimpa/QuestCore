#include "QuestCoreEditorModule.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "FQuestCoreEditorModule"

void FQuestCoreEditorModule::StartupModule()
{
    QuestCoreStyle = MakeShared<FSlateStyleSet>("QuestCoreStyle");
    // Plugin Resources folder
    const FString PluginPath = IPluginManager::Get().FindPlugin(TEXT("QuestCore"))->GetBaseDir();

    QuestCoreStyle->SetContentRoot(FPaths::Combine(PluginPath, TEXT("Resources")));

    // Asset icon
    QuestCoreStyle->Set("QuestDefinition.Icon", new FSlateImageBrush(QuestCoreStyle->RootToContentDir(TEXT("QuestDefinition"), TEXT(".png")), FVector2D(64.0f, 64.0f)));

    FSlateStyleRegistry::RegisterSlateStyle(*QuestCoreStyle);
}

void FQuestCoreEditorModule::ShutdownModule()
{

    if (QuestCoreStyle.IsValid())
    {
        FSlateStyleRegistry::UnRegisterSlateStyle(*QuestCoreStyle);
        QuestCoreStyle.Reset();
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FQuestCoreEditorModule, QuestCoreEditor)