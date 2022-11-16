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
	AEnemyController();         //������

protected:
    void BeginPlay()override;
	virtual void OnPossess(APawn* InPawn) override;     //�� ���ǽ� ȣ�� �Ǵ� �Լ�

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    class UBehaviorTree* BehaviorTree; //�����̺�� Ʈ�� ����

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI")
    class UBehaviorTreeComponent* BehaviorTreeComponent; //�����̺��Ʈ�� ������Ʈ

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    class UBlackboardComponent* BlackboardComponent;    //������ ������Ʈ

    //������ Ű�� ���ٽ� ����� �̸���
    static const FName PlayerKey;
    static const FName StateKey;
    static const FName SelectAttackNumber;
    static const FName Distance;
};
