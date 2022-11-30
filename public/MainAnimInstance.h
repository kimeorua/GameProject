// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

UCLASS()
class CPP_MAGNUSBELLATROR_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	//����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	float Speed; //ĳ������ �ӵ�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	int mode; // ĳ������ ����(�Ϲ�, ��&����, ���)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move")
	UAnimMontage* RollingMontage; //ȸ�� �ִϸ��̼�-> �ִϸ��̼� �������Ʈ���� ���� ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Healing")
	UAnimMontage* PortionMontage; //���� �ִϸ��̼�-> �ִϸ��̼� �������Ʈ���� ���� ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* OnHandCastMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* GreatSwordCastMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* BlockEndMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* DieMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* StunMontage;

public:
	UMainAnimInstance(); // ������

	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //�ִϸ��̼� ������Ʈ �Լ�

	void PlayRollingMontage(); // ȸ�� �ִϸ��̼� ���

	void PlayPortionMontage(); //���� ��� �ִϸ��̼� ���

	void PlayOnHandCastMontage(); //��&���� ���� �ִϸ��̼� ���
	void PlayGreatSwordCastMontage();  //��� ���� �ִϸ��̼� ���
	void PlayBlockEndMontage();

	void PlayHitMontage();
	void PlayDieMontage();
	void playStunMontage();

	UFUNCTION()
	void AnimNotify_UsePortionEnd();
};
