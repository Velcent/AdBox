// Copyright 2014-2016, 2Scars Games. All Rights Reserved.
#include "AdMobJavaWrapper.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"

static jmethodID AdBox_ShowInterstitial = nullptr;
static jmethodID AdBox_ShowBanner = nullptr;
static jmethodID AdBox_CloseBanner = nullptr;
static jmethodID AdBox_LoadRewarded = nullptr;
static jmethodID AdBox_PlayRewarded = nullptr;
static jmethodID AdBox_IsRewardedAvailable = nullptr;
static jmethodID AdBox_IsPrivacyOptionsRequired = nullptr;
static jmethodID AdBox_ShowPrivacyOptions = nullptr;

void InitAdMobJavaMethods()
{
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	if (!Env || !FJavaWrapper::GameActivityClassID)
	{
		return;
	}

	AdBox_ShowInterstitial        = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_ShowAdBoxInterstitial", "(Ljava/lang/String;)V", false);
	AdBox_ShowBanner              = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_ShowAdBoxBanner", "(Ljava/lang/String;)V", false);
	AdBox_CloseBanner             = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_CloseAdBoxBanner", "()V", false);
	AdBox_LoadRewarded            = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_LoadRewardedVideo_AdMob", "(Ljava/lang/String;)V", false);
	AdBox_PlayRewarded            = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_PlayRewardedVideo_AdMob", "()V", false);
	AdBox_IsRewardedAvailable     = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IsRewardedVideoAvailable_AdMob", "()Z", false);
	AdBox_IsPrivacyOptionsRequired = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IsAdBoxPrivacyOptionsRequired", "()Z", false);
	AdBox_ShowPrivacyOptions      = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_ShowAdBoxPrivacyOptions", "()V", false);
}

static void AdBox_CallStringMethod(jmethodID Method, const FString& Param)
{
	if (!Method)
	{
		return;
	}
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		// TScopedJavaObject: local ref otomatis dilepas, gak perlu DeleteLocalRef manual
		auto ParamArg = FJavaHelper::ToJavaString(Env, Param);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, *ParamArg);
	}
}

static void AdBox_CallVoidMethod(jmethodID Method)
{
	if (!Method)
	{
		return;
	}
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method);
	}
}

static bool AdBox_CallBoolMethod(jmethodID Method)
{
	if (!Method)
	{
		return false;
	}
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		return FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, Method);
	}
	return false;
}

void AndroidThunkCpp_ShowAdBoxInterstitial(const FString& AdUnitID) { AdBox_CallStringMethod(AdBox_ShowInterstitial, AdUnitID); }
void AndroidThunkCpp_ShowAdBoxBanner(const FString& AdUnitID)       { AdBox_CallStringMethod(AdBox_ShowBanner, AdUnitID); }
void AndroidThunkCpp_LoadRewardedVideo(const FString& AdUnitID)     { AdBox_CallStringMethod(AdBox_LoadRewarded, AdUnitID); }
void AndroidThunkCpp_CloseAdBoxBanner()                             { AdBox_CallVoidMethod(AdBox_CloseBanner); }
void AndroidThunkCpp_PlayRewardedVideo()                            { AdBox_CallVoidMethod(AdBox_PlayRewarded); }
void AndroidThunkCpp_ShowPrivacyOptions()                           { AdBox_CallVoidMethod(AdBox_ShowPrivacyOptions); }
bool AndroidThunkCpp_IsRewardedVideoAvailable()                     { return AdBox_CallBoolMethod(AdBox_IsRewardedAvailable); }
bool AndroidThunkCpp_IsPrivacyOptionsRequired()                     { return AdBox_CallBoolMethod(AdBox_IsPrivacyOptionsRequired); }
#endif // PLATFORM_ANDROID