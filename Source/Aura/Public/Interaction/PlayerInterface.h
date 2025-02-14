// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP();

	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(int32 NewLevel, int32 LevelUpNum);
	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 NewLevel, int32 LevelUpNum);

	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 LevelUpNum);
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 XP);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints();
	
};
