using UnrealBuildTool;

public class CustomMessage : ModuleRules
{
	public CustomMessage(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{ 

		});

		PrivateDependencyModuleNames.AddRange(new string[] {  	
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"UnrealEd",
	});
	}
}