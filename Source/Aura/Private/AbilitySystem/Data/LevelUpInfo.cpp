// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXp(int32 XP)
{
	int32 level = 1;
	while (XP - LevelUpData[level].LevelUpRequirement > 0 && level < LevelUpData.Num())
	{
		level++;
	}
	return level;
}
