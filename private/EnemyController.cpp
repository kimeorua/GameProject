// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "EnemyStateEnum.h"
#include "Enemy.h"

//접근할 이름에 블랙보드 키값 할당
const FName AEnemyController::PlayerKey(TEXT("Player"));
const FName AEnemyController::StateKey(TEXT("EnemyState"));
const FName AEnemyController::SelectAttackNumber(TEXT("AttackNum"));
const FName AEnemyController::Distance(TEXT("Distance"));

AEnemyController::AEnemyController()
{
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent")); //비헤이비어 트리 생성
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));    //블랙보드 생성
}

void AEnemyController::BeginPlay()
{
    Super::BeginPlay();
    if (IsValid(BehaviorTree))
    {
        RunBehaviorTree(BehaviorTree);  //비헤이비어 트리 사용
        BehaviorTreeComponent->StartTree(*BehaviorTree); //비헤이비어 트리 작동 시작
    }
}

void AEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (IsValid(Blackboard) && IsValid(BehaviorTree))
    {
        Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset); //블랙보드 키값 초기화 실시
    }
}

