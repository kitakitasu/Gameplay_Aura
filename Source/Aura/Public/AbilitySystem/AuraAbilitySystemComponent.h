// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraUserWidget;
class UAuraAbilitySystemComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsDelegate, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGivenSignature, UAuraAbilitySystemComponent*);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FAbilityStatusChanged, const FGameplayTag&/* AbilityTag */, const FGameplayTag&/* StatusTag */);
/**
 * 
 */

UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:

	bool bStartupAbilitiesGiven = false;
	
	FEffectAssetTagsDelegate EffectAssetTags;
	FAbilitiesGivenSignature AbilitiesGivenDelegate;
	FAbilityStatusChanged AbilityStatusChanged;

	static FGameplayTag GetAbilityTagFromSpec(FGameplayAbilitySpec AbilitySpec);
	static FGameplayTag GetInputTagFromAbility(const FGameplayAbilitySpec& Spec);
	static FGameplayTag GetAbilityStatusTag(const FGameplayAbilitySpec& Spec);

	void ForEachAbility(FForEachAbility Delegate);
	
	void AbilityActorInfoSet();
	void AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> StartupAbilities);
	void AddCharacterPassiveAbilities(TArray<TSubclassOf<UGameplayAbility>> StartupAbilities);

	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);

	/* 用来在WidgetController中调用来增加属性 */
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	void UpdateAbilityStatuses(int32 Level);

protected:
	/* AddCharacterAbilities只会在Service中调用，此函数运行后改变ActivateAbilities，正好可以用这个OnRep函数来在Client中监测AddAbilities */
	virtual void OnRep_ActivateAbilities() override;
	
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag);
	
};



