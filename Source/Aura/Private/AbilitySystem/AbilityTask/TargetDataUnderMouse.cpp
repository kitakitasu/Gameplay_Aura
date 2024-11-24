// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	Super::Activate();
	APlayerController* PlayerController = Cast<APlayerController>(GetAvatarActor()->GetInstigatorController());
	FHitResult Hit;
	PlayerController->GetHitResultUnderCursor(ECC_WorldDynamic, false, Hit);
	if (Hit.bBlockingHit)
	{
		FVector Location = Hit.ImpactPoint;
		ValidData.Broadcast(Location);
		DrawDebugSphere(GetWorld(), Location, 20.f, 8, FColor::Blue, false, 5.f);
	}
}

