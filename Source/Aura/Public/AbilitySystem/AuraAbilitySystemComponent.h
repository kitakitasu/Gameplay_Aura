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
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatusTag, const FGameplayTag& InputTag, const FGameplayTag& PreviousInputTag);

/**
 * 
 */

UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:

	bool bStartupAbilitiesGiven = false;
	
	FEffectAssetTagsDelegate EffectAssetTags; //
	FAbilitiesGivenSignature AbilitiesGivenDelegate; //技能获取时，在达到技能规定等级和角色初始化时触发
	FAbilityStatusChanged AbilityStatusChanged; //技能升级时状态改变
	FAbilityEquipped AbilityEquipped; //技能装配更新回调


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

	/*通过标签获取技能描述*/
	bool GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

	UFUNCTION(Server, Reliable) //在服务器处理技能装配，传入技能标签和装配的技能标签
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Slot);
	
	UFUNCTION(Client, Reliable) //在客户端处理技能装配
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

	void ClearSlot(FGameplayAbilitySpec* Spec); //清除技能装配插槽的技能

	void ClearAbilitiesOfSlot(const FGameplayTag& Slot); //根据输入标签，清除技能装配插槽的技能

	static bool AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot); //判断当前技能实例是否处于目标技能装配插槽


	


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



