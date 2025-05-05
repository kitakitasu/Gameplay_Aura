// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/FireBolt.h"

#include "AuraGameplayTags.h"

FString UFireBolt::GetDescription(int32 Level)
{
	return Super::GetDescription(Level);
}

FString UFireBolt::GetNextLevelDescription(int32 Level)
{
	return Super::GetNextLevelDescription(Level);
}

FString UFireBolt::GetDescriptionAtLevel(const int32 Level, const FString& Title)
{
	const float Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		// 标题
		"<Title>%s</>\n"

		// 细节
		"等级： <Level>%i</>\n"
		"技能冷却： <Cooldown>%.1f</>\n"
		"蓝量消耗： <ManaCost>%.1f</>\n\n"

		// 技能描述
		"<Default>发射1颗火球，在发生撞击时产生爆炸，并造成</> <Damage>%.1f</> <Default>点火焰伤害。</>"),

		// 动态修改值
		*Title,
		Level,
		Cooldown,
		ManaCost,
		Damage);
}

FString UFireBolt::GetAbilityName()
{
	return AbilityName;
}
