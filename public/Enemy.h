// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"	

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);			//��������Ʈ ����

UCLASS()
class CPP_MAGNUSBELLATROR_API AEnemy : public ACharacter
{
	GENERATED_BODY()
public:

	AEnemy(); 	// ������
	virtual void Tick(float DeltaTime) override;					//ƽ �Լ�
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;		//�÷��̾� ��ǲ
	virtual void PostInitializeComponents() override; 	//c++ �ʱ�ȭ ��� �� �ʱ�ȭ
	
	void Attack(int num);			//�� ���� �Լ� �Ű������� �޾� �������� ���
	void RangeAttack();			//�� ���� �Լ� �Ű������� �޾� �������� ���
	FOnAttackEndDelegate OnAttackEnd;		//��������Ʈ(�Լ��� �����ϴ� ������ ��������.)

protected:
	virtual void BeginPlay() override;

	//���� ����� ȣ�� �Ǵ� �Լ� -> �ִϸ��̼� ����� ȣ���ϰ�, BTTask_Attack���� AI�۵� ������ ��ȯ�ϵ����ϴ� �Լ�
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float HP; //�����

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = "Attack")
	bool IsAttacking; // ���������� �Ǵ� �ϴ� ����

private:
	UPROPERTY()
	class UEnemyAnimInstance* EnemyAnim; //�ִϸ��̼� ȣ���� ���� ����

};
