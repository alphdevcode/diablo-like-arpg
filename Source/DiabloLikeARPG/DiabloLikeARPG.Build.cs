// Copyright 2024 AlphDevCode. All Rights Reserved.

using UnrealBuildTool;

public class DiabloLikeARPG : ModuleRules
{
	public DiabloLikeARPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
        PublicDependencyModuleNames.AddRange(new string[] { "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "UMG" });
        
        PrivateDependencyModuleNames.AddRange(new string[] { "LoggingSystem" });
    }
}
