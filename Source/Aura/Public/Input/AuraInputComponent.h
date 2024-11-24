// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	template<class UserClass, typename PressFuncType, typename ReleaseFuncType,	typename HoldFuncType>
	void BindAbilityAction(const UAuraInputConfig* InputConfig, UserClass* Object,
		PressFuncType PressFunc, ReleaseFuncType ReleaseFunc, HoldFuncType HeldFunc);
};

template <class UserClass, typename PressFuncType, typename ReleaseFuncType, typename HoldFuncType>
void UAuraInputComponent::BindAbilityAction(const UAuraInputConfig* InputConfig, UserClass* Object,
	PressFuncType PressFunc, ReleaseFuncType ReleaseFunc, HoldFuncType HeldFunc)
{
	check(InputConfig);
	for(const FAuraInputAction Action : InputConfig->AbilityInputActions)
	{
		if(Action.InputAction && Action.InputTag.IsValid())
		{
			if(HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
			if(PressFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressFunc, Action.InputTag);
			}
			if(ReleaseFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleaseFunc, Action.InputTag);
			}
		}
	}
}