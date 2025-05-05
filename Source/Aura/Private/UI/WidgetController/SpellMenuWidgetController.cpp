// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void USpellMenuWidgetController::BroadcastInitalValues()
{
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		BroadcastAbilityInfo(AuraASC);
	}
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		//监听技能状态改变的回调
		AuraASC->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoFromTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		});
		
		//监听技能装配的回调
		AuraASC->AbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);
	}
}

void USpellMenuWidgetController::CancelSpellGlobeSelection()
{
	CancelSpellGlobeSelectionDelegate.Broadcast();

	SelectedAbilityStatusTag = FGameplayTag();
	SelectedAbilityTag = FGameplayTag();
}

void USpellMenuWidgetController::CancelEquippedSpellGlobeSelection()
{
	CancelEquippedSpellGlobeSelectionDelegate.Broadcast();
}

void USpellMenuWidgetController::OnSelectSpellButton(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
{
	SelectedAbilityTag = AbilityTag;
	FAuraGameplayTags Tags = FAuraGameplayTags::Get();
	const FGameplayAbilitySpec* Spec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	if (!StatusTag.IsValid() || AbilityTag.MatchesTag(Tags.Abilities_None) || Spec == nullptr)
	{
		SelectedAbilityStatusTag = Tags.Abilities_Status_Locked;
	}
	else
	{
		SelectedAbilityStatusTag = StatusTag;
	}
	bool bEnableSpendPoints = !SelectedAbilityStatusTag.MatchesTag(Tags.Abilities_Status_Locked) || SpellPoints > 0;
	bool bEnableEquipButton = !SelectedAbilityStatusTag.MatchesTag(Tags.Abilities_Status_Locked);
	
	ButtonEnableDelegate.Broadcast(bEnableEquipButton, bEnableSpendPoints);

	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionByAbilityTag(AbilityTag, Description, NextLevelDescription);
	SpellDescriptionSignature.Broadcast(Description, NextLevelDescription);
}

void USpellMenuWidgetController::EquipButtonPressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	//获取装配技能的类型
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoFromTag(SelectedAbilityTag).AbilityTypeTag;
	if(!SelectedAbilityType.MatchesTagExact(AbilityType)) return; //类型不同无法装配

	//获取装配技能的输入标签
	const FGameplayTag& SelectedAbilityInputTag = GetAuraASC()->GetInputTagFromAbility(*GetAuraASC()->GetSpecFromAbilityTag(SelectedAbilityTag));
	if(SelectedAbilityInputTag.MatchesTagExact(SlotTag)) return; //如果当前技能输入和插槽标签相同，证明已经装配，不需要再处理

	//调用装配技能函数，进行处理
	GetAuraASC()->ServerEquipAbility(SelectedAbilityTag, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	//清除旧插槽的数据
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Enable;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	//更新新插槽的数据
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoFromTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
}

