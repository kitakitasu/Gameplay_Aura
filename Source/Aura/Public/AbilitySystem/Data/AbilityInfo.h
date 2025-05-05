// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

class UAuraGameplayAbility;

USTRUCT(Blueprintable, BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()
	//用来识别技能菜单中的Globe
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag;
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTypeTag;
	//用来识别技能栏中的Globe
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag;
	UPROPERTY(EditDefaultsOnly ,BlueprintReadOnly)
	FGameplayTag CooldownTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> SkillIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAuraGameplayAbility> AbilityClass;
};

/**
 * 
 */
UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation")
	TArray<FAuraAbilityInfo> AbilityInfos;
	
	FAuraAbilityInfo FindAbilityInfoFromTag(FGameplayTag AbilityTag, bool bLogNotFound = true);
};
