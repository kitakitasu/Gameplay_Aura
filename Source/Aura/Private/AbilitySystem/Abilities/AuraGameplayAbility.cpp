// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/AuraPlayerController.h"

//如果使用字符串，需要在字符串前面增加一个L
FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return GetDescriptionAtLevel(Level, AbilityName);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return GetDescriptionAtLevel(Level, FString(L"下一等级"));
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("技能将在角色等级达到<Level>%d</>时解锁"), Level);
}

FString UAuraGameplayAbility::GetDescriptionAtLevel(const int32 Level, const FString& Title)
{
	return FString("技能描述");
}

float UAuraGameplayAbility::GetManaCost(const float Inlevel)
{
	//获取cost的GE，然后通过GE的modifier得到具体消耗的数值
	float ManaCost = 0.0f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				//通过修饰符获取到使用的FScalableFloat并计算传入等级的蓝量消耗
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(Inlevel, ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(const float Inlevel)
{
	float Cooldown = 0.0f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(Inlevel, Cooldown);
	}
	return Cooldown;
}

FString UAuraGameplayAbility::GetAbilityName()
{
	return AbilityName;
}

void UAuraGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (bBlockMove && GetAvatarActorFromActorInfo()->ActorHasTag(FName("Player")))
	{
		if (APawn* Pawn = Cast<APawn>(GetAvatarActorFromActorInfo()))
		{
			AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>(Pawn->GetController());
			PlayerController->SetAutoRunning(false);
		}
	}
}
