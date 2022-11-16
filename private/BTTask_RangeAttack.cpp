// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RangeAttack.h"
#include "Enemy.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RangeAttack::UBTTask_RangeAttack()
{
	bNotifyTick = true; //TickTask()�Լ� ȣ�� ���� ����
	IsAttacking = false; //�ʱ�ȭ
}

EBTNodeResult::Type UBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Enemy)
	{
		return EBTNodeResult::Failed; //����ȯ ���н� ���� ��ȯ
	}
	//int AttackNum = OwnerComp.GetBlackboardComponent()->GetValueAsInt(AEnemyController::SelectAttackNumber); //�����忡�� �����ϴ� ���� �� �о��
	Enemy->RangeAttack(); //�о�� ���� �Ű������� Attack()�Լ� ȣ�� 

	IsAttacking = true; //���������� ����
	Enemy->OnAttackEnd.AddLambda([this]() -> void {IsAttacking = false; }); // OnAttackEnd��������Ʈ�� ���ٽ��� ����Ͽ�,  ������ false�� �����ϴ� �۾� �ǽ�

	return EBTNodeResult::InProgress; //���� ���������� ��ȯ
}

void UBTTask_RangeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking) // ���� ����� ���� ��ȯ Enemy.cpp���Ͽ��� OnMontageEnded(��Ÿ�� ���� �� ȣ��Ǵ� �Լ�)�� ���� �Լ�OnAttackMontageEnded()���� ȣ���
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
