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

	UFUNCTION(BlueprintCallable)
	void Axe1CollisionStart();

	UFUNCTION(BlueprintCallable)
	void Axe1CollisionEnd();

	UFUNCTION(BlueprintCallable)
	void Axe2CollisionStart();

	UFUNCTION(BlueprintCallable)
	void Ax2CollisionEnd();

	UFUNCTION()
	void Axe1OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void Axe2OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Hit();

	void Die();

protected:
	virtual void BeginPlay() override;

	//���� ����� ȣ�� �Ǵ� �Լ� -> �ִϸ��̼� ����� ȣ���ϰ�, BTTask_Attack���� AI�۵� ������ ��ȯ�ϵ����ϴ� �Լ�
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float HP; //�����

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = "Attack")
	bool IsAttacking = false; // ���������� �Ǵ� �ϴ� ����

	//������ ���� ����(����, ������)
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = "Attack")
	class UBoxComponent* AxeCollision1;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = "Attack")
	class UBoxComponent* AxeCollision2;

	FName SokcetName1 = TEXT("ghostbeast_hand_r");
	FName SokcetName2 = TEXT("ghostbeast_hand_l");

private:
	UPROPERTY()
	class UEnemyAnimInstance* EnemyAnim; //�ִϸ��̼� ȣ���� ���� ����

	//���� ������
	float Axe1Damage;
	float Axe2Damage;
};
