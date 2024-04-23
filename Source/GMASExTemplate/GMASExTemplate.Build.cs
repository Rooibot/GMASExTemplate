// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

// ReSharper disable once InconsistentNaming
public class GMASExTemplate : ModuleRules
{
	public GMASExTemplate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTags" });

		PrivateDependencyModuleNames.AddRange(new string[] { "GMCCore", "GMCAbilitySystem", "GMCExtended", "GMCExtendedAnimation" });

		PrivateIncludePaths.AddRange(new string[]
		{
			ModuleDirectory,
			Path.Combine(ModuleDirectory, "Actors"), 
			Path.Combine(ModuleDirectory, "Animation"), 
			Path.Combine(ModuleDirectory, "Components")
		});

	}
}
