// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public ABaseCharacter, public IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;
	/** PlayerState Replication Notification Callback */
	virtual void OnRep_PlayerState() override;

	/** PlayerInterface **/
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void AddToSpellPoints_Implementation(int32 NewLevel, int32 LevelUpNum) override;
	//TODO: 上下的这两个函数原本的逻辑后面发现不适用于减少点数，所以给改成了直接加上LevelUpNum，之后给改好
	virtual void AddToAttributePoints_Implementation(int32 NewLevel, int32 LevelUpNum) override;
	virtual void AddToPlayerLevel_Implementation(int32 LevelUpNum) override;
	virtual int32 FindLevelForXP_Implementation(int32 XP) override;
	virtual int32 GetXP_Implementation() override;
	virtual int32 GetAttributePoints_Implementation() override;
	virtual int32 GetSpellPoints_Implementation() override;
	/** end PlayerInterface **/

	/** ~Begin CombatInterface */
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
	/** ~End CombatInterface */

	virtual void InitAbilityActorInfo() override;
	virtual void InitializeAttributes() override;
	
protected:
	/*
	 * 用于初始化各属性的GameplayEffect class
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Initialization|Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Initialization|Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Initialization|Attributes")
	TSubclassOf<UGameplayEffect> DefaultResistanceAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Initialization|Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY(EditAnywhere, Category = "Level Up")
	TSubclassOf<UGameplayEffect> LevelUpRecover;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagara;

	UFUNCTION(BlueprintNativeEvent)
	FVector GetCameraLocation();
	
private:
	
	void ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& AttributeEffect, float Level) const;

	void LevelUp();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUp();

};
