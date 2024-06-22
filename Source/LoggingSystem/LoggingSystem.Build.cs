// Copyright 2024 AlphDevCode. All Rights Reserved.

using UnrealBuildTool;

public class LoggingSystem : ModuleRules
{
	public LoggingSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "Engine" });

    }
}
