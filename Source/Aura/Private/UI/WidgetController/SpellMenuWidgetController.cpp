// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"

void USpellMenuWidgetController::BroadcastInitalValues()
{
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		BroadcastAbilityInfo(AuraASC);
	}
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	
}

void USpellMenuWidgetController::CancelSpellGlobeSelection()
{
	CancelSpellGlobeSelectionDelegate.Broadcast();
}

void USpellMenuWidgetController::CancelEquippedSpellGlobeSelection()
{
	CancelEquippedSpellGlobeSelectionDelegate.Broadcast();
}
