// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/SummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> USummonAbility::GetSpawnLocation()
{
	TArray<FVector> SpawnLocation;
	const AActor* Character = GetAvatarActorFromActorInfo();
	const FVector Location = Character->GetActorLocation();
	const FVector ForwardDirection = Character->GetActorRotation().Vector();
	const FVector RightSpawnDirection = ForwardDirection.RotateAngleAxis(SpawnSpacerAngle * (NumMinions - 1) / 2, FVector::UpVector);
	const FVector LeftSpawnDirection= ForwardDirection.RotateAngleAxis(-SpawnSpacerAngle * (NumMinions - 1) / 2, FVector::UpVector);
	
	FVector Direction = LeftSpawnDirection;
	for(int32 i = 0; i < NumMinions; i++)
	{
		float SpawnDistance = FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);
		FVector SpawnXYLocation = Location + SpawnDistance * Direction;
		Direction = Direction.RotateAngleAxis(SpawnSpacerAngle, FVector::UpVector);
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, SpawnXYLocation + FVector(0, 0, 400), SpawnXYLocation - FVector(0, 0, 400), ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			SpawnLocation.Add(Hit.Location);
		}
	}
	return SpawnLocation;
}

TSubclassOf<APawn> USummonAbility::GetRandomMinionClass()
{
	const int32 Index = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Index];
}
