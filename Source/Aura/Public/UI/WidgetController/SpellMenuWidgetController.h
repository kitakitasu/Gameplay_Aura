// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCancelSelectionSignature); //技能面板选择一个技能时取消其他的选中状态
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FButtonEnableSignature, bool, bEquipButtonEnable, bool, bUpgradeButtonEnable); //技能面板技能状态
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellDescriptionSignature, FString, SpellDescription, FString, SpellNextLevelDescription); //技能描述


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

	/*选中技能按钮后，技能和下一级描述委托*/
	UPROPERTY(BlueprintAssignable)
	FSpellDescriptionSignature SpellDescriptionSignature; 
	
	/* 在SpellGlobeButton蓝图中调用,广播取消SpellMenu中EquippedSpellGlobeWidget的选中状态 */
	UFUNCTION(BlueprintCallable, Category = "Button")
	void CancelEquippedSpellGlobeSelection();
	
	UPROPERTY(BlueprintAssignable, Category = "Button")
	FCancelSelectionSignature CancelEquippedSpellGlobeSelectionDelegate;

	UFUNCTION(BlueprintCallable, Category = "Button")
	void OnSelectSpellButton(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag);

	UPROPERTY(BlueprintAssignable, Category = "Button")
	FButtonEnableSignature ButtonEnableDelegate;
	
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType); //装配技能按钮按下事件触发
	//监听技能装配后的处理
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const;

	
	UPROPERTY(BlueprintReadWrite, Category = "Spell")
	int32 SpellPoints = 0;


private:
	/*保存选择的能力和状态*/
	FGameplayTag SelectedAbilityTag;
	FGameplayTag SelectedAbilityStatusTag;
	
};
