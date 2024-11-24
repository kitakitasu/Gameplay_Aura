// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(FUWidgetControllerParams Wcp)
{
	PlayerController = Wcp.PlayerController;
	PlayerState = Wcp.PlayerState;
	AbilitySystemComponent = Wcp.AbilitySystemComponent;
	AttributeSet = Wcp.AttributeSet;
}

void UAuraWidgetController::BroadcastInitalValues()
{
	
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}
