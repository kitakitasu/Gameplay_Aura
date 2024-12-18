// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AAuraProjectile::AAuraProjectile()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlay);
	
	SetLifeSpan(LifeTime);
	FlyAudioComponent = UGameplayStatics::SpawnSoundAttached(FlySound, RootComponent);
}

void AAuraProjectile::OnSphereOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//如果碰到Owner或者碰到Owner同阵营Actor就之间return
	if(GetOwner() == OtherActor || UAuraAbilitySystemLibrary::IsFriend(GetOwner(), OtherActor)) return;
	// 如果卡了，FlyAudio可能在生成之前就触发Overlay事件
	if(FlyAudioComponent)
	{
		FlyAudioComponent.Get()->Stop();
	}
	UGameplayStatics::PlaySoundAtLocation(this, BoomSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, BoomNiagara, GetActorLocation());
	bHit = true;
	if(HasAuthority())
	{
		if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
		}
		Destroy();
	}
}

void AAuraProjectile::Destroyed()
{
	if(bHit && !HasAuthority())
	{
		if (FlyAudioComponent) FlyAudioComponent->Stop();
		UGameplayStatics::PlaySoundAtLocation(this, BoomSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, BoomNiagara, GetActorLocation());
	}
	Super::Destroyed();
}

