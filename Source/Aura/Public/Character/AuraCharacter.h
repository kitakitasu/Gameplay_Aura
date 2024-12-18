// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;
	/** PlayerState Replication Notification Callback */
	virtual void OnRep_PlayerState() override;

	/** ~Begin CombatInterface */
	virtual int32 GetPlayerLevel() override;
	/** ~End CombatInterface */

	virtual void InitAbilityActorInfo() override;
	virtual void InitializeAttributes() override;
	
protected:
	/*
	 * 用于初始化各属性的GameplayEffect class
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Initialization|Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Initialization|Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Initialization|Attributes")
	TSubclassOf<UGameplayEffect> DefaultResistanceAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Initialization|Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

private:
	void ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& AttributeEffect, float Level) const;

	//不知道为什么VitalAttribute不能和Primary,Secondary属性一起初始化，实在找不到bug出在哪了，
	//这里就直接暴力循环初始化直到初始化成功，就停止循环
	FTimerHandle TimerHandle;
	void InitVitalAttributeInfo();
};
