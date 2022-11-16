// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_RandomNumber.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_RandomNumber::UBTService_RandomNumber()
{
	NodeName = (TEXT("RandomAttack")); //표시될 노드 이름
	Interval = 0.5f;											// 반복되는 시간
	//bNotifyBecomeRelevant = true;				//OnBecomeRelevant() 함수 사용
}

void UBTService_RandomNumber::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	int x = 0;	//랜덤한 숫자를 저장할 변수
	x = FMath::RandRange(1, 100);		// 랜덤 값 저장


	if (x <= 60)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyController::SelectAttackNumber, 1); //60%의 확률
	}
	else if (x <= 100)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyController::SelectAttackNumber, 2); //40%의 확률
	}
}
