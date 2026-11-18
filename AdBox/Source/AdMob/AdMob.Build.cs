// Copyright 2014-2016, 2Scars Games. All Rights Reserved.

using System.IO;

namespace UnrealBuildTool.Rules
{
	public class AdMob : ModuleRules
	{
		public AdMob(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

			PublicDependencyModuleNames.AddRange(new string[]
			{
				"Core",
				"CoreUObject",
				"Engine"
			});

			if (Target.Platform == UnrealTargetPlatform.Android)
			{
				PrivateDependencyModuleNames.Add("Launch");

				string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
				AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "AdMob_APL.xml"));
			}
		}
	}
}