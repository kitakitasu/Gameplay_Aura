// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"

class UAuraAttributeSet;
class UCharacterClassInfo;

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400.f, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//给客户端初始化GAS
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	
	if(AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		AuraHUD->InitOverlayWidget(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
	}
	
	InitializeAttributes();
}

void AAuraCharacter::InitializeAttributes()
{
	if (!HasAuthority()) return;
	ApplyGameplayEffectToSelf(DefaultPrimaryAttributes, 1);
	ApplyGameplayEffectToSelf(DefaultSecondaryAttributes, 1);
	ApplyGameplayEffectToSelf(DefaultResistanceAttributes, 1);
	ApplyGameplayEffectToSelf(DefaultVitalAttributes, 1);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::InitVitalAttributeInfo, 0.1f, true);
}

void AAuraCharacter::ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& AttributeEffect, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(AttributeEffect);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(AttributeEffect, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void AAuraCharacter::InitVitalAttributeInfo()
{
	ApplyGameplayEffectToSelf(DefaultResistanceAttributes, 1);
	ApplyGameplayEffectToSelf(DefaultVitalAttributes, 1);
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	if (AS->GetHealth() > 0.f)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}

