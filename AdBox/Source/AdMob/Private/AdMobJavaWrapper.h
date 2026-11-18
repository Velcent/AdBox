// Copyright 2014-2016, 2Scars Games. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

#if PLATFORM_ANDROID
void InitAdMobJavaMethods();
void AndroidThunkCpp_ShowAdBoxInterstitial(const FString& AdUnitID);
void AndroidThunkCpp_ShowAdBoxBanner(const FString& AdUnitID);
void AndroidThunkCpp_CloseAdBoxBanner();
void AndroidThunkCpp_LoadRewardedVideo(const FString& AdUnitID);
void AndroidThunkCpp_PlayRewardedVideo();
bool AndroidThunkCpp_IsRewardedVideoAvailable();
bool AndroidThunkCpp_IsPrivacyOptionsRequired();
void AndroidThunkCpp_ShowPrivacyOptions();
#endif