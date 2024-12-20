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
		SpawnLocation.Add(Location + SpawnDistance * Direction);
		Direction = Direction.RotateAngleAxis(SpawnSpacerAngle, FVector::UpVector);
	}
	return SpawnLocation;
}
