// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DiabloLikeARPG : ModuleRules
{
	public DiabloLikeARPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "UMG" });
    }
}
