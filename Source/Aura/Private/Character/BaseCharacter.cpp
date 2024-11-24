// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "ShaderPrintParameters.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void ABaseCharacter::InitializeAttributes() const
{
	ApplyGameplayEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyGameplayEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyGameplayEffectToSelf(DefaultVitalAttributes, 1.f);
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

