// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->SetGenerateOverlapEvents(false);
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FVector ABaseCharacter::GetWeaponSocketLocation()
{
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::InitAbilityActorInfo()
{
}

void ABaseCharacter::InitializeAttributes()
{
	//
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::InitCharacterAttributeInfo, 0.2f, true);
}

void ABaseCharacter::AddCharacterAbilities()
{
	if(!HasAuthority()) return;
	UAuraAbilitySystemComponent* AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraAbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
}

void ABaseCharacter::ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> AttributeEffect, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(AttributeEffect);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(AttributeEffect, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ABaseCharacter::InitCharacterAttributeInfo()
{
	const AAuraGameModeBase* GameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	const TObjectPtr<UCharacterClassInfo> CharacterClassInfo = GameMode->CharacterClassInfo;
	const TSubclassOf<UGameplayEffect> PrimaryAttributeEffect = CharacterClassInfo->GetCharacterDefaultInfoByClass(CharacterClass).PrimaryAttributes;
	const TSubclassOf<UGameplayEffect> SecondaryAttributeEffect = CharacterClassInfo->SecondAttributes;
	const TSubclassOf<UGameplayEffect> VitalAttributeEffect = CharacterClassInfo->VitalAttributes;
	
	FGameplayEffectContextHandle PrimaryContextHandle = AbilitySystemComponent->MakeEffectContext();
	PrimaryContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle PrimarySpecHandle = AbilitySystemComponent->MakeOutgoingSpec(PrimaryAttributeEffect, GetPlayerLevel(), PrimaryContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryContextHandle = AbilitySystemComponent->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SecondarySpecHandle = AbilitySystemComponent->MakeOutgoingSpec(SecondaryAttributeEffect, GetPlayerLevel(), SecondaryContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalContextHandle = AbilitySystemComponent->MakeEffectContext();
	VitalContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle VitalSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(VitalAttributeEffect, GetPlayerLevel(), VitalContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get());
	
}

