// Copyright 2014-2016, 2Scars Games. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AdMobFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct FAdMobRewardItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "RewardItem")
	FString type;

	UPROPERTY(BlueprintReadOnly, Category = "RewardItem")
	int32 amount = 0;
};

DECLARE_DYNAMIC_DELEGATE(FRewardDispatcher);
DECLARE_DYNAMIC_DELEGATE_OneParam(FRewardEarnedDispatcher, FAdMobRewardItem, reward);

UCLASS(MinimalAPI)
class UAdMobFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Banner
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AdBox Show Banner"), Category = "Plugin|AdBox")
	static void ShowAdBoxBanner(int32 adID);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AdBox Close Banner"), Category = "Plugin|AdBox")
	static void CloseAdBoxBanner();

	// Interstitial
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AdBox Show Interstitial"), Category = "Plugin|AdBox")
	static void ShowAdBoxInterstitial(int32 adID);

	// Rewarded
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AdBox Load Rewarded Video"), Category = "Plugin|AdBox")
	static void LoadRewardedVideo(int32 Index, const FRewardDispatcher& onRewardedAdLoaded, const FRewardDispatcher& onRewardedAdFailedToLoad);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AdBox Play Rewarded Video"), Category = "Plugin|AdBox")
	static void PlayRewardedVideo(const FRewardDispatcher& onRewardedAdShowed, const FRewardDispatcher& onRewardedAdFailedToShow, const FRewardDispatcher& onRewardedAdDismissed, const FRewardEarnedDispatcher& onUserEarnedReward);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "AdBox Is Rewarded Video Available"), Category = "Plugin|AdBox")
	static bool IsRewardedVideoAvailable();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "AdBox Get Reward Item"), Category = "Plugin|AdBox")
	static FAdMobRewardItem getRewardItem();

	// Privacy / UMP (baru)
	UFUNCTION(BlueprintPure, meta = (DisplayName = "AdBox Is Privacy Options Required"), Category = "Plugin|AdBox")
	static bool IsPrivacyOptionsRequired();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AdBox Show Privacy Options"), Category = "Plugin|AdBox")
	static void ShowPrivacyOptions();

	// Internal, dipanggil dari JNI
	static void ExecuteOnGameThread(TFunction<void()> Function);
	static void callRewardDelegates(int32 index);
	static void callUserEarnedReward(FString rewardType, int32 amount);

	static FRewardDispatcher RAL, RAF, ORAS, ORAFS, ORAD;
	static FRewardEarnedDispatcher OUER;
	static FAdMobRewardItem rewardItem;

private:
	static bool GetAdUnitID(int32 adID, FString& OutAdUnitID);
};