// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AuraAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AAuraAIController::AAuraAIController()
{
	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>("BehaviorTree");

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackBoardComponent");
}
