// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class CPP_MAGNUSBELLATROR_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()
	
	//ī�޶� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) // �����Ϳ��� Ȯ�� ����, �������Ʈ����Getȣ�� ����
	class USpringArmComponent* CameraBoom;		// �������� -> ī�޶� �޸� ��

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;		//ī�޶�

public:
	// Sets default values for this character's properties
	AMainCharacter();			//������

	// �ִ� �ӵ� �� �ּ� �ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	float MaxSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	float LowSpeed;

	// ȸ�� �� �̵� �Ÿ�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move") // �����Ϳ��� Ȯ�� ����, �������Ʈ����Get,Setȣ�� ����
	float MoveDistance;

	// ���� ������ ������ Ȯ���ϴ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	bool IsRolling;
	
	// ���� �뽬 ������ Ȯ�� �ϴ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	bool IsDash;

	// ���� ����(�Ϲ�, ��&����, ���)����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SetMode")
	int Mode;

	//���콺 ��Ŭ�� �ߴ��� Ȯ�� �ϴ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	bool bLMBdawn;

	// ���� ������ Ȯ�� �ϴ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	bool IsAttack;

	//�̹� �������� ���¿��� ���콺Ŭ���� �Ͽ����� Ȯ�� �ϴ� ����
	bool AttackButtonWhenAttack;

	// ���� �޺� ��ȣ
	int ComboNum;

	// ��� ������ Ȯ�� �ϴ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	bool IsBlock;

	//��&���� ���� �ִϸ��̼�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack") // �����Ϳ��� ���� ���� �������Ʈ���� Get ȣ�� ����
	class UAnimMontage* SwordAndShiledMontage;

	// ��� ���� �ִϸ��̼�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	class UAnimMontage* GreatSwordMontage;

	//��� �ִϸ��̼�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	class UAnimMontage* BlockMontage;

	// ��� ���� ���¸� ���� Ÿ�̸�
	FTimerHandle timer;

	//Q��ų �ִϸ��̼� ����(����� ���� ��)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	class UAnimMontage* QSkillMontage;

	//E��ų �ִϸ��̼� ����(����� ���� ��)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	class UAnimMontage* ESkillMontage;

	// ��ų ��������� Ȯ�� �ϴ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	bool ActivateSkill;

	//���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* MainWeapon;

	// ���� ������ ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portion")
	int PortionNum;

	// ȸ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float Healing;

	// ���� ���� ��� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool IsHealing;

	//HP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float HP;

	//SP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float SP;

	// ������ �� ���Ǵ� ���׹̳�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float RollingSP;

	//�ִ� ü��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float MaxHP;

	//�ִ� ���¹̳�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float MaxSP;

	// ȸ���Ǵ� ���׹̳� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float RecoverySP;

	//���׹̳� ȸ�� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsRecovery;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input -> �÷��̾��� �Է� ó��, BindAxis(�� �Է�), BindAction(�׼� �Է�)
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; } // ������ �� ��ȯ
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; } // ī�޶� ��ȯ

	// �յ�, �¿� �̵� �Լ�
	void MoveForward(float Value);
	void MoveRight(float Value);

	// �뽬&�뽬 ���� �Լ�
	void Dash();
	void DashStop();

	// ������ & ������ ���� �Լ�
	UFUNCTION(BlueprintCallable) // �������Ʈ���� ȣ�� ����
	void Rolling();
	UFUNCTION(BlueprintCallable)
	void RollingEnd();

	// ���� ��� ���� �Լ�
	void SetMode(int ModeNum);

	//���콺 Press, Released �Լ�
	void LMBdawn();
	FORCEINLINE void LMBup() { bLMBdawn = false; } // FORCEINLINE - > �𸮾󿡼� ����ϴ� �ζ��� �Լ� �ڵ尡 ª�� �Լ��� ���

	// ���� �� ���� ���� �Լ�
	void Attack();
	UFUNCTION(BlueprintCallable) // ���� ����� �ִϸ��̼�BP���� ��Ƽ���̸� ���� ȣ�� ��
	void AttackEnd();

	//��� ����-��� ��-��� ���� �Լ�
	void BlockStart();
	void Block();
	void BlockEnd();

	// ���� �޺� üũ�� �Լ�, �ִϸ��̼�BP���� ��Ƽ���̸� ���� ȣ��
	UFUNCTION(BlueprintCallable)
	void CheackCombo();

	// Q,E��ų �۵� �Լ�
	void QSkillActive();
	void ESkillActive();

	// ��ų ���� �Լ� �ִϸ��̼�BP ���� ��Ƽ���̸� ���� ȣ��
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void DeactivateSkill() { ActivateSkill = false; }

	//���� ���
	void UsePortion();
	void DecreasePortion();

	//���¹̳� ȸ�� �Լ�
	UFUNCTION(BlueprintCallable)
	void Recovery();

	// ���� ����
	void SettingWeapon(AWeapon* Weapon);

	// ���� �ߵ�
	void MagicOn();
};
