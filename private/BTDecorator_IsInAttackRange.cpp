// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "MainCharacter.h"
#include "Enemy.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange() //데코레이터 -> 조건식, 다른 노드에 부착되어 트리의 분기나 단일 노드의 실행 여부를 결정.
{
	NodeName = TEXT("CheackInAttackRange");		//노드 이름
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory); 

	auto ControllingPawn = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn()); // 조종하고있는 폰 정보 변수에 저장
	if (ControllingPawn == nullptr)
	{
		return false;
	}
	//현재 추적중인 플레이어 변수에 저장
	auto Target = Cast<AMainCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyController::PlayerKey)); 
	if (Target == nullptr)
	{
		return false;
	}
		

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 300.0f); // 현제 추적중인 적과의 거리가 300(3m)가 되면 true 아니면 false반환
	return bResult;
}
