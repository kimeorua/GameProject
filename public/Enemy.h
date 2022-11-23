// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"	

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);			//델리게이트 선언

UCLASS()
class CPP_MAGNUSBELLATROR_API AEnemy : public ACharacter
{
	GENERATED_BODY()
public:
	AEnemy(); 	// 생성자
	virtual void Tick(float DeltaTime) override;					//틱 함수
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;		//플레이어 인풋
	virtual void PostInitializeComponents() override; 	//c++ 초기화 허용 및 초기화
	
	void Attack(int num);			//적 공격 함수 매개변수를 받아 랜덤으로 출력
	void RangeAttack();			//적 공격 함수 매개변수를 받아 랜덤으로 출력
	FOnAttackEndDelegate OnAttackEnd;		//델리게이트(함수를 저장하는 변수라 생각하자.)

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

	//공격 종료시 호출 되는 함수 -> 애니메이션 종료시 호출하고, BTTask_Attack에서 AI작동 성공을 반환하도록하는 함수
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float HP; //생명력

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = "Attack")
	bool IsAttacking = false; // 공격중인지 판단 하는 변수

	//도끼의 공격 판정(왼쪽, 오른쪽)
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = "Attack")
	class UBoxComponent* AxeCollision1;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = "Attack")
	class UBoxComponent* AxeCollision2;

	FName SokcetName1 = TEXT("ghostbeast_hand_r");
	FName SokcetName2 = TEXT("ghostbeast_hand_l");

private:
	UPROPERTY()
	class UEnemyAnimInstance* EnemyAnim; //애니메이션 호출을 위한 변수

	//도끼 데미지
	float Axe1Damage;
	float Axe2Damage;
};
