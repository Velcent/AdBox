// Copyright 2014-2016, 2Scars Games. All Rights Reserved.
#include "AdMobFunctionLibrary.h"
#include "AdMobPrivatePCH.h"
#include "AdMobJavaWrapper.h"
#include "Misc/ConfigCacheIni.h"
#include "Async/Async.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#endif

DEFINE_LOG_CATEGORY(LogAdBox);

FRewardDispatcher UAdMobFunctionLibrary::RAL;
FRewardDispatcher UAdMobFunctionLibrary::RAF;
FRewardDispatcher UAdMobFunctionLibrary::ORAS;
FRewardDispatcher UAdMobFunctionLibrary::ORAFS;
FRewardDispatcher UAdMobFunctionLibrary::ORAD;
FRewardEarnedDispatcher UAdMobFunctionLibrary::OUER;
FAdMobRewardItem UAdMobFunctionLibrary::rewardItem;

bool UAdMobFunctionLibrary::GetAdUnitID(int32 adID, FString& OutAdUnitID)
{
	TArray<FString> AdUnitIDs;
	GConfig->GetArray(TEXT("/Script/AndroidRuntimeSettings.AndroidRuntimeSettings"), TEXT("AdMobAdUnitIDs"), AdUnitIDs, GEngineIni);

	if (AdUnitIDs.IsValidIndex(adID) && !AdUnitIDs[adID].IsEmpty())
	{
		OutAdUnitID = AdUnitIDs[adID];
		return true;
	}

	// Fallback setting lama (single ad unit)
	if (GConfig->GetString(TEXT("/Script/AndroidRuntimeSettings.AndroidRuntimeSettings"), TEXT("AdMobAdUnitID"), OutAdUnitID, GEngineIni) && !OutAdUnitID.IsEmpty())
	{
		return true;
	}

	UE_LOG(LogAdBox, Warning, TEXT("AdBox: AdMobAdUnitIDs[%d] gak ada di DefaultEngine.ini"), adID);
	return false;
}

void UAdMobFunctionLibrary::ShowAdBoxInterstitial(int32 adID)
{
#if PLATFORM_ANDROID
	FString AdUnitID;
	if (GetAdUnitID(adID, AdUnitID))
	{
		AndroidThunkCpp_ShowAdBoxInterstitial(AdUnitID);
	}
#endif
}

void UAdMobFunctionLibrary::ShowAdBoxBanner(int32 adID)
{
#if PLATFORM_ANDROID
	FString AdUnitID;
	if (GetAdUnitID(adID, AdUnitID))
	{
		AndroidThunkCpp_ShowAdBoxBanner(AdUnitID);
	}
#endif
}

void UAdMobFunctionLibrary::CloseAdBoxBanner()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_CloseAdBoxBanner();
#endif
}

void UAdMobFunctionLibrary::LoadRewardedVideo(int32 Index, const FRewardDispatcher& onRewardedAdLoaded, const FRewardDispatcher& onRewardedAdFailedToLoad)
{
	RAL = onRewardedAdLoaded;
	RAF = onRewardedAdFailedToLoad;

#if PLATFORM_ANDROID
	FString AdUnitID;
	if (GetAdUnitID(Index, AdUnitID))
	{
		AndroidThunkCpp_LoadRewardedVideo(AdUnitID);
	}
	else
	{
		ExecuteOnGameThread([]() { RAF.ExecuteIfBound(); });
	}
#endif
}

void UAdMobFunctionLibrary::PlayRewardedVideo(const FRewardDispatcher& onRewardedAdShowed, const FRewardDispatcher& onRewardedAdFailedToShow, const FRewardDispatcher& onRewardedAdDismissed, const FRewardEarnedDispatcher& onUserEarnedReward)
{
	ORAS = onRewardedAdShowed;
	ORAFS = onRewardedAdFailedToShow;
	ORAD = onRewardedAdDismissed;
	OUER = onUserEarnedReward;

#if PLATFORM_ANDROID
	AndroidThunkCpp_PlayRewardedVideo();
#endif
}

bool UAdMobFunctionLibrary::IsRewardedVideoAvailable()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_IsRewardedVideoAvailable();
#else
	return false;
#endif
}

FAdMobRewardItem UAdMobFunctionLibrary::getRewardItem()
{
	return rewardItem;
}

bool UAdMobFunctionLibrary::IsPrivacyOptionsRequired()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_IsPrivacyOptionsRequired();
#else
	return false;
#endif
}

void UAdMobFunctionLibrary::ShowPrivacyOptions()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_ShowPrivacyOptions();
#endif
}

// --------------- Callback dari Java (JNI) ---------------
#if PLATFORM_ANDROID
static void AdBox_HandleRewardCallback(int32 Index)
{
	UAdMobFunctionLibrary::callRewardDelegates(Index);
}

static void AdBox_HandleUserEarnedReward(JNIEnv* Env, jstring RewardType, int32 Amount)
{
	FString Type = FJavaHelper::FStringFromLocalRef(Env, RewardType);
	UAdMobFunctionLibrary::callUserEarnedReward(Type, Amount);
}

// UE5: package Java-nya com.epicgames.unreal (GameActivity & NativeActivity)
JNI_METHOD void Java_com_epicgames_unreal_GameActivity_CPPRewardAd(JNIEnv* Env, jclass Clazz, jint Index)
{
	AdBox_HandleRewardCallback(Index);
}

JNI_METHOD void Java_com_epicgames_unreal_GameActivity_CPPUserEarnRewardAd(JNIEnv* Env, jclass Clazz, jstring RewardType, jint Amount)
{
	AdBox_HandleUserEarnedReward(Env, RewardType, Amount);
}

// Legacy UE4, biar kompatibel kalau ada yang masih pakai package lama
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_CPPRewardAd(JNIEnv* Env, jclass Clazz, jint Index)
{
	AdBox_HandleRewardCallback(Index);
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_CPPUserEarnRewardAd(JNIEnv* Env, jclass Clazz, jstring RewardType, jint Amount)
{
	AdBox_HandleUserEarnedReward(Env, RewardType, Amount);
}
#endif // PLATFORM_ANDROID

void UAdMobFunctionLibrary::callRewardDelegates(int32 index)
{
	switch (index)
	{
	case 1: ExecuteOnGameThread([]() { RAL.ExecuteIfBound(); });   break; // rewarded loaded
	case 2: ExecuteOnGameThread([]() { RAF.ExecuteIfBound(); });   break; // rewarded failed to load
	case 3: ExecuteOnGameThread([]() { ORAS.ExecuteIfBound(); });  break; // rewarded showed
	case 4: ExecuteOnGameThread([]() { ORAFS.ExecuteIfBound(); }); break; // rewarded failed to show
	case 5: ExecuteOnGameThread([]() { ORAD.ExecuteIfBound(); });  break; // rewarded dismissed
	default: break;
	}
}

void UAdMobFunctionLibrary::callUserEarnedReward(FString rewardType, int32 amount)
{
	rewardItem.type = rewardType;
	rewardItem.amount = amount;
	ExecuteOnGameThread([]() { OUER.ExecuteIfBound(rewardItem); });
}

void UAdMobFunctionLibrary::ExecuteOnGameThread(TFunction<void()> Function)
{
	if (IsInGameThread())
	{
		Function();
	}
	else
	{
		AsyncTask(ENamedThreads::GameThread, MoveTemp(Function));
	}
}