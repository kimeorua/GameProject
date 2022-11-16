// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class CPP_MAGNUSBELLATROR_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();         //생성자

protected:
    void BeginPlay()override;
	virtual void OnPossess(APawn* InPawn) override;     //폰 빙의시 호출 되는 함수

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    class UBehaviorTree* BehaviorTree; //비헤이비어 트리 에셋

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI")
    class UBehaviorTreeComponent* BehaviorTreeComponent; //비헤이비어트리 컴포넌트

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    class UBlackboardComponent* BlackboardComponent;    //블랙보드 컴포넌트

    //블랙보드 키에 접근시 사용할 이름들
    static const FName PlayerKey;
    static const FName StateKey;
    static const FName SelectAttackNumber;
    static const FName Distance;
};
