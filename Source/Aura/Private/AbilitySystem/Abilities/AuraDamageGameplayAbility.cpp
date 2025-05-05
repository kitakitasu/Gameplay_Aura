// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

float UAuraDamageGameplayAbility::GetDamageByDamageType(float Level, FGameplayTag DamageTag)
{
	for (auto Pair : DamageType)
	{
		if (Pair.Key.MatchesTag(DamageTag))
		{
			return Pair.Value.GetValueAtLevel(Level);
		}
	}
	return 0.0f;
}

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (auto Pair : DamageType)
	{
		const float DamageValue = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, DamageValue);
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}
