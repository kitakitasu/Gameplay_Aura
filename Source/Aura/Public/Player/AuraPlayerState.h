// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnValueChangedSignature, int32);

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; };

	FORCEINLINE int32 GetPlayerLevel() const { return PlayerLevel; };

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void SetLevel(int32 NewLevel);
	UFUNCTION(BlueprintCallable)
	void AddToLevel(int32 InLevel);

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
	UFUNCTION(BlueprintCallable)
	void SetXP(int32 NewXP);
	UFUNCTION(BlueprintCallable)
	void AddToXP(int32 InXP);
	UFUNCTION(BlueprintCallable)
	inline int32 GetXP() const { return XP; }

	UFUNCTION(BlueprintCallable)
	void SetSpellPoints(int32 NewSpellPoints);
	UFUNCTION(BlueprintCallable)
	void AddToSpellPoints(int32 InSpellPoints);
	UFUNCTION(BlueprintCallable)
	inline int32 GetSpellPoints() const { return SpellPoints; }

	UFUNCTION(BlueprintCallable)
	void SetAttributePoints(int32 NewAttributePoints);
	UFUNCTION(BlueprintCallable)
	void AddToAttributePoints(int32 InAttributePoints);
	UFUNCTION(BlueprintCallable)
	inline int32 GetAttributePoints() const { return AttributePoints; }
	
	FOnValueChangedSignature OnXPChangedDelegate;
	FOnValueChangedSignature OnLevelChangedDelegate;
	FOnValueChangedSignature OnAttributePointsChangedDelegate;
	FOnValueChangedSignature OnSpellPointsChangedDelegate;
	
protected:
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY()
	UAttributeSet* AttributeSet;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 PlayerLevel = 1;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
	int32 SpellPoints = 0;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	UFUNCTION()
	void OnRep_XP(int32 OldXP);
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
};

