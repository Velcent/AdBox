// Copyright 2014-2016, 2Scars Games. All Rights Reserved.
#include "AdMobPrivatePCH.h"
#include "AdMobJavaWrapper.h"

class FAdMobModule : public IAdMobModule
{
	virtual void StartupModule() override
	{
#if PLATFORM_ANDROID
		InitAdMobJavaMethods();
#endif
	}

	virtual void ShutdownModule() override
	{
	}
};

IMPLEMENT_MODULE(FAdMobModule, AdMob)