// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RangeAttack.h"
#include "Enemy.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RangeAttack::UBTTask_RangeAttack()
{
	bNotifyTick = true; //TickTask()함수 호출 가능 선언
	IsAttacking = false; //초기화
}

EBTNodeResult::Type UBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Enemy)
	{
		return EBTNodeResult::Failed; //형변환 실패시 실패 반환
	}
	//int AttackNum = OwnerComp.GetBlackboardComponent()->GetValueAsInt(AEnemyController::SelectAttackNumber); //블랙보드에서 공격하는 패턴 값 읽어옴
	Enemy->RangeAttack(); //읽어온 값을 매개변수로 Attack()함수 호출 

	IsAttacking = true; //공격중으로 변경
	Enemy->OnAttackEnd.AddLambda([this]() -> void {IsAttacking = false; }); // OnAttackEnd델리게이트에 람다식을 사용하여,  변수를 false로 변경하는 작업 실시

	return EBTNodeResult::InProgress; //현제 진행중으로 반환
}

void UBTTask_RangeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking) // 공격 종료시 성공 반환 Enemy.cpp파일에서 OnMontageEnded(몽타지 종료 시 호출되는 함수)에 붙인 함수OnAttackMontageEnded()에서 호출됨
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
