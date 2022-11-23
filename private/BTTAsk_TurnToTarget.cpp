// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTAsk_TurnToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy.h"
#include "EnemyController.h"
#include "MainCharacter.h"

UBTTAsk_TurnToTarget::UBTTAsk_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTAsk_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    auto Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
    if (nullptr == Enemy)
    {
        return EBTNodeResult::Failed;
    }

    auto Target = Cast<AMainCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyController::PlayerKey));
    if (nullptr == Target)
    {
        return EBTNodeResult::Failed;
    }

    FVector LookVector = (Target->GetActorLocation()) - (Enemy->GetActorLocation());
    LookVector.Z = 0.0f;
    FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
    Enemy->SetActorRotation(FMath::RInterpTo(Enemy->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

    return EBTNodeResult::Succeeded;
}
