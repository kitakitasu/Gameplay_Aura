// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextWidget.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class AURA_API UDamageTextWidget : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(FGameplayTag DamageType, float Damage, bool bIsBlocked, bool bIsCritical);
};
