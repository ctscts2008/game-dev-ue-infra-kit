// Copyright Epic Games, Inc. All Rights Reserved.


using UnrealBuildTool;

public class ExtendEditorStyle : ModuleRules
{
	public ExtendEditorStyle(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"SlateCore",
			"Slate",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
		});
	}
}