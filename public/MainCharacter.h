// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class CPP_MAGNUSBELLATROR_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()
	
	//카메라 세팅
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) // 에디터에서 확인 가능, 블루프린트에서Get호출 가능
	class USpringArmComponent* CameraBoom;		// 스프링암 -> 카메라가 달릴 곳

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;		//카메라

public:
	// Sets default values for this character's properties
	AMainCharacter();			//생성자

	// 최대 속도 및 최소 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	float MaxSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	float LowSpeed;

	// 회피 시 이동 거리
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move") // 에디터에서 확인 가능, 블루프린트에서Get,Set호출 가능
	float MoveDistance;

	// 현제 구르기 중인지 확인하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	bool IsRolling;
	
	// 현제 대쉬 중인지 확인 하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	bool IsDash;

	// 현제 상태(일반, 검&방패, 대검)상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SetMode")
	int Mode;

	//마우스 좌클릭 했는지 확인 하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	bool bLMBdawn;

	// 공격 중인지 확인 하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	bool IsAttack;

	//이미 공격중인 상태에서 마우스클릭을 하였는지 확인 하는 변수
	bool AttackButtonWhenAttack;

	// 현제 콤보 번호
	int ComboNum;

	// 방어 중인지 확인 하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	bool IsBlock;

	//검&방패 공격 애니메이션
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack") // 에디터에서 설정 가능 블루프린트에서 Get 호출 가능
	class UAnimMontage* SwordAndShiledMontage;

	// 대검 공격 애니메이션
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	class UAnimMontage* GreatSwordMontage;

	//방어 애니메이션
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	class UAnimMontage* BlockMontage;

	// 방어 유지 상태를 위한 타이머
	FTimerHandle timer;

	//Q스킬 애니메이션 변수(무기와 연동 됨)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	class UAnimMontage* QSkillMontage;

	//E스킬 애니메이션 변수(무기와 연동 됨)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	class UAnimMontage* ESkillMontage;

	// 스킬 사용중인지 확인 하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	bool ActivateSkill;

	//매인 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* MainWeapon;

	// 현제 포션의 수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portion")
	int PortionNum;

	// 회복량
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float Healing;

	// 현제 포션 사용 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool IsHealing;

	//HP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float HP;

	//SP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float SP;

	// 구르기 시 사용되는 스테미나
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float RollingSP;

	//최대 체력
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float MaxHP;

	//최대 스태미나
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float MaxSP;

	// 회복되는 스테미나 양
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float RecoverySP;

	//스테미나 회복 가능 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsRecovery;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input -> 플레이어의 입력 처리, BindAxis(축 입력), BindAction(액션 입력)
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; } // 스프링 암 반환
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; } // 카메라 반환

	// 앞뒤, 좌우 이동 함수
	void MoveForward(float Value);
	void MoveRight(float Value);

	// 대쉬&대쉬 종료 함수
	void Dash();
	void DashStop();

	// 구르기 & 구르기 종료 함수
	UFUNCTION(BlueprintCallable) // 블루프린트에서 호출 가능
	void Rolling();
	UFUNCTION(BlueprintCallable)
	void RollingEnd();

	// 현제 모드 셋팅 함수
	void SetMode(int ModeNum);

	//마우스 Press, Released 함수
	void LMBdawn();
	FORCEINLINE void LMBup() { bLMBdawn = false; } // FORCEINLINE - > 언리얼에서 사용하는 인라인 함수 코드가 짧은 함수에 사용

	// 공격 및 공격 종료 함수
	void Attack();
	UFUNCTION(BlueprintCallable) // 공격 종료는 애니메이션BP에서 노티파이를 통해 호출 함
	void AttackEnd();

	//방어 시작-방어 중-방어 종료 함수
	void BlockStart();
	void Block();
	void BlockEnd();

	// 현제 콤보 체크용 함수, 애니메이션BP에서 노티파이를 통해 호출
	UFUNCTION(BlueprintCallable)
	void CheackCombo();

	// Q,E스킬 작동 함수
	void QSkillActive();
	void ESkillActive();

	// 스킬 종료 함수 애니메이션BP 에서 노티파이를 통해 호출
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void DeactivateSkill() { ActivateSkill = false; }

	//포션 사용
	void UsePortion();
	void DecreasePortion();

	//스태미나 회복 함수
	UFUNCTION(BlueprintCallable)
	void Recovery();

	// 무기 셋팅
	void SettingWeapon(AWeapon* Weapon);

	// 마법 발동
	void MagicOn();
};
