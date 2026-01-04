// Copyright Epic Games, Inc. All Rights Reserved.


using UnrealBuildTool;

public class ExtendMenuBase : ModuleRules
{
	public ExtendMenuBase(ReadOnlyTargetRules Target) : base(Target)
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
			 "ExtendEditorStyle",
			 "ExtendEditorCommand",
			 "CustomMessage",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			
			"ToolMenus",
			"ToolWidgets",
	
		});
	}
}