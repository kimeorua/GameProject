// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "EnemyController.h"
#include "Enemy.h"
#include "MainCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"


UBTService_Detect::UBTService_Detect() //BT서비스 -> 태스크, 컴포짓 부착 분기가 실행되는 동안 주기만큼 실행, 보통 블랙보드 의 확인 및 업데이트에 사용. 
{
	NodeName = TEXT("Detect"); //표시되는 노드 이름
	Interval = 0.05f; //반복 주기
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // ControllingPawn에 현제 조종하는 Pawn 저장
	if (ControllingPawn == nullptr)
	{
		return;
	}
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation(); //후에 사용할OverlapMultiByChannel에서 사용할 중점을 적 캐릭터의 좌표로 설정
	float DetectRadius = 600.0f;

	if (World == nullptr)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults; //오버랩 결과
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
			AMainCharacter* Main = Cast<AMainCharacter>(OverlapResult.GetActor()); //bResult가 true면(오버랩되면) 감지된것으로 판단 플레이어를 블랙보드에 기입
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
	else			//플레이어가 오버랩되지 않음 ->감지 되지 않았으므로, 블랙보드에 nullptr로 기입
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyController::PlayerKey, nullptr);
	}
	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);

}
