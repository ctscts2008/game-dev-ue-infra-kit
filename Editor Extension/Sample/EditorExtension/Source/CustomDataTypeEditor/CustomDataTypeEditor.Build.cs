// Copyright Epic Games, Inc. All Rights Reserved.


using UnrealBuildTool;

public class CustomDataTypeEditor  : ModuleRules
{
	public CustomDataTypeEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"CustomDataType",
			"SlateCore",
			"Slate",
			"UnrealEd",
			"AssetTools",
			"EditorFramework"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
		});
	}
}