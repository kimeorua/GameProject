// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CPP_MagnusBellatror : ModuleRules
{
	public CPP_MagnusBellatror(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay",
			"NavigationSystem", 
			"AIModule", 
			"GameplayTasks" });
	}
}
