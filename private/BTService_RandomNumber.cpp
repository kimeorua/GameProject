// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_RandomNumber.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_RandomNumber::UBTService_RandomNumber()
{
	NodeName = (TEXT("RandomAttack")); //ǥ�õ� ��� �̸�
	Interval = 0.5f;											// �ݺ��Ǵ� �ð�
	//bNotifyBecomeRelevant = true;				//OnBecomeRelevant() �Լ� ���
}

void UBTService_RandomNumber::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	int x = 0;	//������ ���ڸ� ������ ����
	x = FMath::RandRange(1, 100);		// ���� �� ����


	if (x <= 60)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyController::SelectAttackNumber, 1); //60%�� Ȯ��
	}
	else if (x <= 100)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AEnemyController::SelectAttackNumber, 2); //40%�� Ȯ��
	}
}
