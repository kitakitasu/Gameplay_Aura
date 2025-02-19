// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.generated.h"

class UAuraAbilitySystemComponent;
class UAbilityInfo;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, Info);

/**
 * 
 */
USTRUCT(BlueprintType)
struct FUWidgetControllerParams
{
	GENERATED_BODY()
	
	FUWidgetControllerParams() {};
	FUWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* Asc, UAttributeSet* As)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(Asc), AttributeSet(As) {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet;
};

UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(FUWidgetControllerParams Wcp);
	/* 在Widget初始化时调用，在WidgetController中广播以获取所需值 */
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitalValues();
	/* 在HUD中初始化此WidgetController的同时调用,绑定WidgetController中的各Delegate依赖 */
	virtual void BindCallbacksToDependencies();

	UPROPERTY(BlueprintAssignable, Category = "GAS|Ability")
	FAbilityInfoSignature AbilityInfoDelegate;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
	void BroadcastAbilityInfo(UAuraAbilitySystemComponent* AuraASC);

	
};
