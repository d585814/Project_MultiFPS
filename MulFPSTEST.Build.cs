// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MulFPSTEST : ModuleRules
{
	public MulFPSTEST(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","UMG", "OnlineSubsystem", "OnlineSubsystemSteam" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
    
}

