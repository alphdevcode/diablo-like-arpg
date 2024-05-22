// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DiabloLikeARPGTarget : TargetRules
{
	public DiabloLikeARPGTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		//DefaultBuildSettings = BuildSettingsVersion.V2;
		DefaultBuildSettings = BuildSettingsVersion.V5;
	    //IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("DiabloLikeARPG");
	}
}
