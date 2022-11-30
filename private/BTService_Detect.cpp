// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "EnemyController.h"
#include "Enemy.h"
#include "MainCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"


UBTService_Detect::UBTService_Detect() //BT���� -> �½�ũ, ������ ���� �бⰡ ����Ǵ� ���� �ֱ⸸ŭ ����, ���� ������ �� Ȯ�� �� ������Ʈ�� ���. 
{
	NodeName = TEXT("Detect"); //ǥ�õǴ� ��� �̸�
	Interval = 0.05f; //�ݺ� �ֱ�
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // ControllingPawn�� ���� �����ϴ� Pawn ����
	if (ControllingPawn == nullptr)
	{
		return;
	}
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation(); //�Ŀ� �����OverlapMultiByChannel���� ����� ������ �� ĳ������ ��ǥ�� ����
	float DetectRadius = 600.0f;

	if (World == nullptr)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults; //������ ���
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel
	(
		OverlapResults,
		Center, 
		FQuat::Identity, 
		ECollisionChannel::ECC_GameTraceChannel2, 
		FCollisionShape::MakeSphere(DetectRadius), 
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const OverlapResult : OverlapResults)
		{
			AMainCharacter* Main = Cast<AMainCharacter>(OverlapResult.GetActor()); //bResult�� true��(�������Ǹ�) �����Ȱ����� �Ǵ� �÷��̾ �����忡 ����
			AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
			if (Main && Main->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyController::PlayerKey, Main);
				Main->CombetEnemy = Enemy;
				Enemy->CombetMain = Cast<AMainCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyController::PlayerKey));
				return;
			}
		}
	}
	else			//�÷��̾ ���������� ���� ->���� ���� �ʾ����Ƿ�, �����忡 nullptr�� ����
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyController::PlayerKey, nullptr);
	}
	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);

}
