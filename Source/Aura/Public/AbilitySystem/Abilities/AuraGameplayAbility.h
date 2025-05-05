// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	//激活这个Ability的输入按键的InputTag
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	virtual FString GetDescription(int32 Level); //获取当前等级的技能描述
	virtual FString GetNextLevelDescription(int32 Level); //获取技能下一等级的技能描述
	static  FString GetLockedDescription(int32 Level); //获取锁定技能描述


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Move")
	bool bBlockMove = true;

	float GetManaCost(const float Inlevel);
	float GetCooldown(const float Inlevel);

	virtual FString GetAbilityName();
	FString AbilityName = FString(L"技能名");

	virtual FString GetDescriptionAtLevel(const int32 Level, const FString& Title);
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
