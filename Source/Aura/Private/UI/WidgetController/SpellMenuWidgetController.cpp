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
		AuraASC->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoFromTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		});
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
	FAuraGameplayTags Tags = FAuraGameplayTags::Get();
	const FGameplayAbilitySpec* Spec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTag(Tags.Abilities_None) || Spec == nullptr)
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
}

