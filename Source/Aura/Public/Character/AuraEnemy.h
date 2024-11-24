// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()
public:
	/** ~Begin EnemyInterface */
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
	/** ~End EnemyInterface */
	AAuraEnemy();
	/** ~Begin CombatInterface */
	virtual int32 GetPlayerLevel() override;
	/** ~End CombatInterface*/
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 PlayerLevel = 1;
};
