// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCancelSelectionSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FButtonEnableSignature, bool, bEquipButtonEnable, bool, bUpgradeButtonEnable);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitalValues() override;
	virtual void BindCallbacksToDependencies() override;

	/* 在SpellGlobeButton蓝图中调用,广播取消SpellMenu中SpellGlobeWidget的选中状态 */
	UFUNCTION(BlueprintCallable, Category = "Button")
	void CancelSpellGlobeSelection();
	
	UPROPERTY(BlueprintAssignable, Category = "Button")
	FCancelSelectionSignature CancelSpellGlobeSelectionDelegate;
	
	/* 在SpellGlobeButton蓝图中调用,广播取消SpellMenu中EquippedSpellGlobeWidget的选中状态 */
	UFUNCTION(BlueprintCallable, Category = "Button")
	void CancelEquippedSpellGlobeSelection();
	
	UPROPERTY(BlueprintAssignable, Category = "Button")
	FCancelSelectionSignature CancelEquippedSpellGlobeSelectionDelegate;

	UFUNCTION(BlueprintCallable, Category = "Button")
	void OnSelectSpellButton(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag);

	UPROPERTY(BlueprintAssignable, Category = "Button")
	FButtonEnableSignature ButtonEnableDelegate;

	UPROPERTY(BlueprintReadWrite, Category = "Spell")
	int32 SpellPoints = 0;


private:
	/*保存选择的能力和状态*/
	FGameplayTag SelectedAbilityTag;
	FGameplayTag SelectedAbilityStatusTag;
	
};
