// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_RandomNumber.generated.h"

/**
 * 
 */
UCLASS()
class CPP_MAGNUSBELLATROR_API UBTService_RandomNumber : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_RandomNumber();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
