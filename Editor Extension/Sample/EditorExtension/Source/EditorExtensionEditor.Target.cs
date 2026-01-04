// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class EditorExtensionEditorTarget : TargetRules
{
	public EditorExtensionEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.AddRange(new string[]
		{
	
			"EditorExtension",
			"ExtendMenuBase",
			"CustomDataType",
			"CustomDataTypeEditor",
			"CustomSlate",
		});
	}
}
