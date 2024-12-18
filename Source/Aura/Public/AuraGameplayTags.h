// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */
struct FAuraGameplayTags
{
public:
	static FAuraGameplayTags& Get() { return GameplayTags; };
	static void InitializeNativeGameplayTags();
	/*
	 * Primary Attributes
	 */
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	/*
	 * Secondary Attributes
	 */
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	/*
	 * Vital Attributes
	 */
	FGameplayTag Attributes_Vital_Health;
	FGameplayTag Attributes_Vital_Mana;
	/*
	 * Input Tags
	 */
	FGameplayTag Input_RMB;
	FGameplayTag Input_LMB;
	FGameplayTag Input_1;
	FGameplayTag Input_2;
	FGameplayTag Input_3;
	FGameplayTag Input_4;
	FGameplayTag Input_Q;
	FGameplayTag Input_W;
	FGameplayTag Input_E;
	FGameplayTag Input_R;
	FGameplayTag Input_A;
	FGameplayTag Input_S;
	/*
	 * Damage Resistance
	 */
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;
	/*
	 * DamageType Tags
	 */
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	/*
	 * Ability Tags
	 */
	FGameplayTag Abilities_MeleeAttack;
	/*
	 * Effect Tags
	 */
	FGameplayTag Effects_HitReact;

	/*
	 * Montage Tags
	 */
	FGameplayTag Montage_Attack_LeftHand;
	FGameplayTag Montage_Attack_RightHand;
	FGameplayTag Montage_Attack_Weapon;

	TMap<FGameplayTag, FGameplayTag> DamageTypeToResistance;
private:
  static FAuraGameplayTags GameplayTags;
 
};