// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTAsk_TurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class CPP_MAGNUSBELLATROR_API UBTTAsk_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTAsk_TurnToTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
