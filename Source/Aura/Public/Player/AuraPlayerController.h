// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/EnemyInterface.h"
#include "AuraPlayerController.generated.h"

class UDamageTextWidget;
class USplineComponent;
class UAuraAbilitySystemComponent;
struct FGameplayTag;
class UAuraInputConfig;
class UAuraInputComponent;
class UInputMappingContext;
class UInputAction;
class IEnemyInterface;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	
	UFUNCTION(Client, Reliable)
    void ShowDamageText(FGameplayTag DamageType, float DamageValue, ACharacter* TargetCharacter, bool bIsBlocked, bool bIsCritical);
	/* 用来在Ability触发时取消自动寻路，在AuraGameplayAbility中调用 */
	void SetAutoRunning(bool bShouldAutoRun);
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

private:

	void AutoRunning();
	
	void CursorTrace();
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	//用于类内使用
	UAuraAbilitySystemComponent* GetASC();
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> AuraInputConfig;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;

	FHitResult CursorHit;
	
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	/*
	* Running Variables
	* 要使用自动寻路需要在Build.cs配置中加上NavigationSystem, 客户端要使用需要在ProjectSettings/NavigationSystem/中开启Client Side什么玩意
	*/
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "AutoRunning")
	float ShotPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting =false;
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 30.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageTextWidget> DamageTextClass;
};
