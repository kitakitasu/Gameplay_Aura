// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Interaction/PlayerInterface.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//OnGameplayEffectAppliedDelegateToSelf只会在服务端调用，所以这里绑定RPC函数来保证客户端也能调用
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> StartupAbilities)
{
	for(const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if(const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
			//把能力添加到ASC中
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast(this);
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(TArray<TSubclassOf<UGameplayAbility>> StartupAbilities)
{
	for(const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Locked);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast(this);
	}
}

void UAuraAbilitySystemComponent::MulticastUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag);
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(FGameplayAbilitySpec AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromAbility(const FGameplayAbilitySpec& Spec)
{
	for (FGameplayTag Tag : Spec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FAuraGameplayTags::Get().Input))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityStatusTag(const FGameplayAbilitySpec& Spec)
{
	for (FGameplayTag Tag : Spec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::ForEachAbility(FForEachAbility Delegate)
{
	//Used to stop us from removing abilities from an ability system component while we're iterating through the abilities
	FScopedAbilityListLock ActiveScopeLock(*this); //防止因为某些能力被block之类的原因而导致下面的ActivatableAbilities中没有
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if(!InputTag.IsValid()) return;
	for(FGameplayAbilitySpec AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if(!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(!InputTag.IsValid()) return;
	for(FGameplayAbilitySpec AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopedLoc(*this);
	for (FGameplayAbilitySpec Spec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : Spec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &Spec;
			}
		}
	}
	return nullptr;
}

void UAuraAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(this);
	for (FAuraAbilityInfo Info : AbilityInfo->AbilityInfos)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequirement) continue;
		//给予AbilityInfo中有但是Character没有的能力
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.AbilityClass, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			//强制更新Ability的复制
			MarkAbilitySpecDirty(AbilitySpec);
			MulticastUpdateAbilityStatus(Info.AbilityTag, FAuraGameplayTags::Get().Abilities_Status_Eligible);
			
		}
	}
}

bool UAuraAbilitySystemComponent::GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription,
	FString& OutNextLevelDescription)
{
	//如果当前技能处于锁定状态，将无法获取到对应的技能描述
	if(FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if(UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec->Ability))
		{
			const float Level = AbilitySpec->Level;
			OutDescription = AuraAbility->GetDescription(Level);
			OutNextLevelDescription = AuraAbility->GetNextLevelDescription(Level + 1);
			return true;
		}
	}

	//如果技能是锁定状态，将显示锁定技能描述
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	OutDescription = UAuraGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoFromTag(AbilityTag).LevelRequirement);
	OutNextLevelDescription = FString();
	return  false;
}

void UAuraAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag Slot = GetInputTagFromAbility(*Spec);
    Spec->DynamicAbilityTags.RemoveTag(Slot);
    MarkAbilitySpecDirty(*Spec);
}

void UAuraAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for(FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if(AbilityHasSlot(&Spec, Slot))
		{
			ClearSlot(&Spec);
		}
	}
}

bool UAuraAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot)
{
	for(FGameplayTag Tag : Spec->DynamicAbilityTags)
	{
		if(Tag.MatchesTagExact(Slot))
		{
			return true;
		}
	}
	return false;
}

void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag,
                                                                    const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	AbilityEquipped.Broadcast(AbilityTag, Status, Slot, PreviousSlot); //在客户端将更新后的标签广播
}

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
                                                                    const FGameplayTag& Slot)
{
	if(FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FGameplayTag& PrevSlot = GetInputTagFromAbility(*AbilitySpec); //技能之前装配的插槽
		const FGameplayTag& Status = GetAbilityStatusTag(*AbilitySpec); //当前技能的状态标签

		//判断技能的状态，技能状态只有在已装配或者已解锁的状态才可以装配
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		if(Status == GameplayTags.Abilities_Status_Equipped || Status == GameplayTags.Abilities_Status_Enable)
		{
			ClearAbilitiesOfSlot(Slot); //通过技能的输入标签清除掉插槽的技能
			ClearSlot(AbilitySpec); //清除掉当前技能的输入标签
			AbilitySpec->DynamicAbilityTags.AddTag(Slot); //将目标插槽的输入标签添加到技能实例的动态标签容器中

			//如果状态标签是已解锁，我们需要将其修改为已装配状态
			if(!Status.MatchesTagExact(GameplayTags.Abilities_Status_Enable))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Enable);	
				AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Equipped);
			}
			ClientEquipAbility(AbilityTag, Status, Slot, PrevSlot);
			MarkAbilitySpecDirty(*AbilitySpec); //立即将其复制到每个客户端
		}
	}
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData PayLoad;
	PayLoad.EventTag = AttributeTag;
	PayLoad.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, PayLoad);

	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetAvatarActor()))
	{
		PlayerInterface->AddToAttributePoints_Implementation(1, -1);
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}
