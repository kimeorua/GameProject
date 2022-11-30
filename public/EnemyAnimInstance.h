// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyStateEnum.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CPP_MAGNUSBELLATROR_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	float Speed; //캐릭터의 속도

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* AttackMontageWeak; //약 공격 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* AttackMontageStrong; //강 공격 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* RangeAttackMontageStrong; //강 공격 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* HitMontage; //강 공격 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* StunMontage; //강 공격 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* DieMontage; //강 공격 몽타주


public:
	UEnemyAnimInstance(); // 생성자

	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //애니메이션 업데이트 함수

	void PlayAttackMontage(int num); // 재생할 애니매이션의 번호를 받아 애니메이션 재생

	void PlayRangeAttackMontage();

	void PlayHitMontage();

	void PlayStunMontage();

	void PlayDieMontage();
};
