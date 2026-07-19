using UnrealBuildTool;

public class QuestCoreEditor : ModuleRules
{
	public QuestCoreEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.Add(ModuleDirectory);
		PrivateIncludePaths.Add(ModuleDirectory);

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"QuestCoreRuntime"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",

			"Slate",
			"SlateCore",

			"Projects",

			"UnrealEd",
			"AssetDefinition",
			"AssetTools"
		});
	}
}
