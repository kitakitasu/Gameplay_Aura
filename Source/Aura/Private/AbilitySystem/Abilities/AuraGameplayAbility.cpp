// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "Player/AuraPlayerController.h"

void UAuraGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (bBlockMove && GetAvatarActorFromActorInfo()->ActorHasTag(FName("Player")))
	{
		if (APawn* Pawn = Cast<APawn>(GetAvatarActorFromActorInfo()))
		{
			AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>(Pawn->GetController());
			PlayerController->SetAutoRunning(false);
		}
	}
}
