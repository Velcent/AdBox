// Copyright 2014-2016, 2Scars Games. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class IAdMobModule : public IModuleInterface
{
public:
	static inline IAdMobModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IAdMobModule>("AdMob");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("AdMob");
	}
};