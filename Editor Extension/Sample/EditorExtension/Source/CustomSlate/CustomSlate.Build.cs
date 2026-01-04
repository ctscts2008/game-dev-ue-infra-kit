using UnrealBuildTool;

public class CustomSlate : ModuleRules
{
	public CustomSlate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[]
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"ExtendEditorStyle",
			"SlateCore",
			"Slate",
			"ToolMenus",
		});
		PrivateDependencyModuleNames.AddRange(new string[] { "ExtendEditorStyle" });
	}
	
	
	
	
	
}