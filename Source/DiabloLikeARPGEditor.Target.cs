// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DiabloLikeARPGEditorTarget : TargetRules
{
	public DiabloLikeARPGEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		//DefaultBuildSettings = BuildSettingsVersion.V2;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		//IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("DiabloLikeARPG");
	}
}
