// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "BaseCharacter.generated.h"

class UCharacterClassInfo;
struct FCharacterClassDefaultInfo;
enum class ECharacterClass : uint8;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class AURA_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UFUNCTION(BlueprintCallable)
	UAttributeSet* GetAttributeSet() const { return AttributeSet; };

	/* CombatInterface */
	virtual FVector GetWeaponSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual void Die() override;
	/* end CombatInterface */
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY()
	UAttributeSet* AttributeSet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	/*
	 * 需要在蓝图中编辑以初始化的变量
	 */
	/* 攻击 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTaggedMontage> AttackMontages;
	UPROPERTY(EditAnywhere, Category = "Initialization|Combat")
	FName WeaponTipSocketName;
	UPROPERTY(EditAnywhere, Category = "Initialization|Combat")
	FName LeftHandSocketName;
	UPROPERTY(EditAnywhere, Category = "Initialization|Combat")
	FName RightHandSocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Initialization|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization|Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UFUNCTION(Category = "Initialization|Attributes")
	virtual void InitializeAttributes();
	UFUNCTION()
	void AddCharacterAbilities();
};
