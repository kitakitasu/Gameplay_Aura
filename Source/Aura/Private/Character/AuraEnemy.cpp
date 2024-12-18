// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AI/AuraAIController.h"
#include "Aura/Aura.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/AuraUserWidget.h"


AAuraEnemy::AAuraEnemy()
{
	bReplicates = true;
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystem_Component");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	InitializeHealthBar();
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}
	
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
		this,
		&AAuraEnemy::OnHitReactTagChanged
	);
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(!HasAuthority()) return ;
	AuraAIController = Cast<AAuraAIController>(NewController);
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsRangedAttacker"),CharacterClass != ECharacterClass::Warrior);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsGiantAttacker"), bIsGiant);
}

void AAuraEnemy::InitializeHealthBar()
{
	if(UAuraUserWidget* AuraHealthBar = Cast<UAuraUserWidget>(HealthBar->GetWidget()))
	{
		AuraHealthBar->SetWidgetController(this);
	}
	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	check(AuraAttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChangeDelegate.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChangeDelegate.Broadcast(Data.NewValue);
		}
	);
	OnHealthChangeDelegate.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChangeDelegate.Broadcast(AuraAttributeSet->GetMaxHealth());
}

void AAuraEnemy::OnHitReactTagChanged(const FGameplayTag CallBackTag, int32 NewCount)
{
	const bool bHitReact = NewCount > 0;

	if (bHitReact)
	{
		WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	if (AuraAIController && HasAuthority()) //客户端里的AIController是空的
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReact);
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	if (HasAuthority())
	{
		InitializeAttributes();
	}
}

void AAuraEnemy::InitializeAttributes()
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, PlayerLevel, AbilitySystemComponent);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ThisClass::InitializeVitalAttributes, 0.1f, true);
}

void AAuraEnemy::InitializeVitalAttributes()
{
	if (!HasAuthority()) return;
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, PlayerLevel, AbilitySystemComponent);
	if (UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(GetAttributeSet()))
	{
		float Health = AuraAttributeSet->GetHealth();
		if (Health != 0)
		{
			GetWorldTimerManager().ClearTimer(TimerHandle);
		}
	}
}

void AAuraEnemy::Die()
{
	Super::Die();
	SetLifeSpan(LifeSpan);
	MulticastDie();
}

void AAuraEnemy::MulticastDie_Implementation()
{
	DissolveMesh();
}

/*
 * EnemyInterface 
 */
void AAuraEnemy::HighLightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighLightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

/*
 * CombatInterface
 */
int32 AAuraEnemy::GetPlayerLevel()
{
	return PlayerLevel;
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* TargetActor)
{
	CombatTarget = TargetActor;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation()
{
	return CombatTarget;
}
