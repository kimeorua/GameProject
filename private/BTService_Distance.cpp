// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Distance.h"
#include "EnemyController.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainCharacter.h"

UBTService_Distance::UBTService_Distance()
{
	NodeName = TEXT("Distance");
	Interval = 0.1f;
}

void UBTService_Distance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}
	auto Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	auto Target = Cast<AMainCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyController::PlayerKey));

	if (nullptr == Target)
	{
		return;
	}
		
	//bool bResult;
	float Distance = 600.0;
	Distance = Target->GetDistanceTo(ControllingPawn);
	//bResult = (Target->GetDistanceTo(ControllingPawn) <= 1000);

	//OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemyController::Distance, bResult);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(AEnemyController::Distance, Distance);
}
