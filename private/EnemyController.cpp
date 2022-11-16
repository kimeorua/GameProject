// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "EnemyStateEnum.h"
#include "Enemy.h"

//������ �̸��� ������ Ű�� �Ҵ�
const FName AEnemyController::PlayerKey(TEXT("Player"));
const FName AEnemyController::StateKey(TEXT("EnemyState"));
const FName AEnemyController::SelectAttackNumber(TEXT("AttackNum"));
const FName AEnemyController::Distance(TEXT("Distance"));

AEnemyController::AEnemyController()
{
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent")); //�����̺�� Ʈ�� ����
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));    //������ ����
}

void AEnemyController::BeginPlay()
{
    Super::BeginPlay();
    if (IsValid(BehaviorTree))
    {
        RunBehaviorTree(BehaviorTree);  //�����̺�� Ʈ�� ���
        BehaviorTreeComponent->StartTree(*BehaviorTree); //�����̺�� Ʈ�� �۵� ����
    }
}

void AEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (IsValid(Blackboard) && IsValid(BehaviorTree))
    {
        Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset); //������ Ű�� �ʱ�ȭ �ǽ�
    }
}

