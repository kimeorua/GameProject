// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EnemyStateEnum.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum  class EnemyState : uint8
{
	ES_IDLE UMETA(DisplayName = "IDLE"),
	ES_CHASE UMETA(DisplayName = "CHASE"),
	ES_ATTACK UMETA(DisplayName = "ATTACK"),
	ES_DEAD UMETA(DisplayName = "DEAD"),

	ES_MAX
};

UCLASS()
class CPP_MAGNUSBELLATROR_API UEnemyStateEnum : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
