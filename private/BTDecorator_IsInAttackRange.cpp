// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "MainCharacter.h"
#include "Enemy.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange() //���ڷ����� -> ���ǽ�, �ٸ� ��忡 �����Ǿ� Ʈ���� �б⳪ ���� ����� ���� ���θ� ����.
{
	NodeName = TEXT("CheackInAttackRange");		//��� �̸�
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory); 

	auto ControllingPawn = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn()); // �����ϰ��ִ� �� ���� ������ ����
	if (ControllingPawn == nullptr)
	{
		return false;
	}
	//���� �������� �÷��̾� ������ ����
	auto Target = Cast<AMainCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyController::PlayerKey)); 
	if (Target == nullptr)
	{
		return false;
	}
		

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 300.0f); // ���� �������� ������ �Ÿ��� 300(3m)�� �Ǹ� true �ƴϸ� false��ȯ
	return bResult;
}
