// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"


FAuraAbilityInfo UAbilityInfo::FindAbilityInfoFromTag(FGameplayTag AbilityTag, bool bLogNotFound)
{
	for (const FAuraAbilityInfo Info : AbilityInfos)
	{
		if (Info.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("failed to find ability info from ability tag[%s]"), *AbilityTag.ToString());
	}
	return FAuraAbilityInfo();
}
