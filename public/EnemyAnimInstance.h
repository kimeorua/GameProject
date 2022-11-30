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
	float Speed; //ĳ������ �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* AttackMontageWeak; //�� ���� ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* AttackMontageStrong; //�� ���� ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* RangeAttackMontageStrong; //�� ���� ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* HitMontage; //�� ���� ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* StunMontage; //�� ���� ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* DieMontage; //�� ���� ��Ÿ��


public:
	UEnemyAnimInstance(); // ������

	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //�ִϸ��̼� ������Ʈ �Լ�

	void PlayAttackMontage(int num); // ����� �ִϸ��̼��� ��ȣ�� �޾� �ִϸ��̼� ���

	void PlayRangeAttackMontage();

	void PlayHitMontage();

	void PlayStunMontage();

	void PlayDieMontage();
};
