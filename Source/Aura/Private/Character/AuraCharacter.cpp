// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"

class UAuraAttributeSet;
class UCharacterClassInfo;

AAuraCharacter::AAuraCharacter()
{
	LevelUpNiagara = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagara");
	LevelUpNiagara->SetupAttachment(RootComponent);
	LevelUpNiagara->bAutoActivate = false;
	
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

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
	{
		AuraPlayerState->AddToXP(InXP);
	}
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 NewLevel, int32 LevelUpNum)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->AddToAttributePoints(LevelUpNum);
	
	//把升过的等级获取的点数累加
	/*while (LevelUpNum)
	{
		int32 Points = AuraPlayerState->LevelUpInfo->LevelUpData[NewLevel - LevelUpNum].AttributePointReward;
		AuraPlayerState->AddToAttributePoints(Points);
		LevelUpNum -= 1;
	}*/
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 NewLevel, int32 LevelUpNum)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	AuraPlayerState->AddToSpellPoints(LevelUpNum);
	//把升过的等级获取的点数累加
	/*while (LevelUpNum)
	{
		int32 Points = AuraPlayerState->LevelUpInfo->LevelUpData[NewLevel - LevelUpNum].SpellPointReward;
		AuraPlayerState->AddToSpellPoints(Points);
		LevelUpNum -= 1;
	}*/
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 LevelUpNum)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(LevelUpNum);
	LevelUp();
}

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 XP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->FindLevelForXp(XP);
}

int32 AAuraCharacter::GetXP_Implementation()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetXP();
}

int32 AAuraCharacter::GetAttributePoints_Implementation()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoints_Implementation()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoints();
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

FVector AAuraCharacter::GetCameraLocation_Implementation()
{
	return FVector();
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

void AAuraCharacter::LevelUp()
{
	MulticastLevelUp();
}

void AAuraCharacter::MulticastLevelUp_Implementation()
{
	if (IsValid(LevelUpNiagara))
	{
		FRotator ToCameraRotation = (GetCameraLocation() - LevelUpNiagara->GetComponentLocation()).Rotation();
		LevelUpNiagara->SetWorldRotation(ToCameraRotation);
		LevelUpNiagara->Activate(true);
	}
	if (IsValid(LevelUpRecover))
	{
		ApplyGameplayEffectToSelf(LevelUpRecover, GetPlayerLevel());
	}
	else
	{
		UE_LOG(LogAura, Warning, TEXT("Player LevelUpRecover Effect doesn't set"));
	}
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

