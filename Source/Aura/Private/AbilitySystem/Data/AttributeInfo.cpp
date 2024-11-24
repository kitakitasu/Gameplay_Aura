// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, const bool bLogNotFound) const
{
	for (FAuraAttributeInfo Info : AttributeInfos)
	{
		if(AttributeTag.MatchesTagExact(Info.AttributeTag))
		{
			return Info;
		}
	}
	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find info for AttributeTag : %s"), *AttributeTag.ToString());
	}
	return FAuraAttributeInfo();
}
