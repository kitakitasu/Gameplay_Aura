// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UAnimMontage;
class UBehaviorTree;
class UWidgetComponent;
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

	UPROPERTY(BlueprintAssignable, Category = "Widget")
	FOnAttributeChangedSignature OnHealthChangeDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Widget")
	FOnAttributeChangedSignature OnMaxHealthChangeDelegate;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	void OnHitReactTagChanged(const FGameplayTag CallBackTag, int32 NewCount);
	
protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	/*
	 * 属性初始化
	 */
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeAttributes() override;
	//Vital的初始化不知道为什么在前两帧会失败，只好把这个的初始化延迟，这个函数是在BaseEnemy蓝图中调用了
	UFUNCTION(BlueprintCallable, Category = "Initialization")
	void InitializeVitalAttributes();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 PlayerLevel = 1;

	/*
	 * AI
	 */
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	
	/*
	 * 死亡行为
	 */
	virtual void Die() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death")
	float LifeSpan = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	TObjectPtr<UMaterialInstance> DissolveMesh_MI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	TObjectPtr<UMaterialInstance> DissolveWeaponMesh_MI;
	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
	void DissolveMesh();

private:
	float WalkSpeed;

	void InitializeHealthBar();
};
