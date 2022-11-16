// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon.h"
#include "MainAnimInstance.h"

// Sets default values
// ���� �ʱ�ȭ �� ������Ʈ ����
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ���� �ʱ�ȭ//
	MaxSpeed = 800.0f;
	LowSpeed = 400.0f;
	MoveDistance = 750.0f;

	ComboNum = 0;
	Mode = 0;

	PortionNum = 3;

	MaxHP = 100.0f;
	MaxSP = 100.0f;

	HP = MaxHP / 2;
	SP = MaxSP;

	Healing = 25.0f;
	RollingSP = 50.0f;

	RecoverySP = 1.5f;

	IsRolling = false;
	IsDash = false;
	bLMBdawn = false;
	IsAttack = false;
	AttackButtonWhenAttack = false;
	IsBlock = false;
	ActivateSkill = false;
	bIsRecovery = true;

	SwordAndShiledMontage = nullptr;
	GreatSwordMontage = nullptr;
	BlockMontage = nullptr;

	QSkillMontage = nullptr;
	ESkillMontage = nullptr;
	MainWeapon = nullptr;

	IsHealing = false;


	// ������Ʈ //
	//
	// ĸ�� ������Ʈ ũ�� ����
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));//������ �� ������Ʈ ����
	CameraBoom->SetupAttachment(RootComponent); // RootComponent�� ����
	CameraBoom->TargetArmLength = 400.0f; //�ȱ��� 400���� ����
	CameraBoom->bUsePawnControlRotation = true; // ��Ʈ�ѷ� ȸ�� ���
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")); // ī�޶� ������Ʈ ����
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // ī�޶� ����(������ ��)
	FollowCamera->bUsePawnControlRotation = false;		// ��Ʈ�� ȸ�� ��� X
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	//�� �Է�//
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);

	//�׼�//

	//�뽬
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMainCharacter::Dash);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &AMainCharacter::DashStop);
	//����
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainCharacter::LMBdawn);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AMainCharacter::LMBup);
	//���
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AMainCharacter::BlockStart);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &AMainCharacter::BlockEnd);
	//��ų���
	PlayerInputComponent->BindAction("QSkill", IE_Pressed, this, &AMainCharacter::QSkillActive);
	PlayerInputComponent->BindAction("ESkill", IE_Pressed, this, &AMainCharacter::ESkillActive);

	//���� ���ñ�
	PlayerInputComponent->BindAction("Drinking", IE_Pressed, this, &AMainCharacter::UsePortion);

	//���� ���
	PlayerInputComponent->BindAction("Magic", IE_Pressed, this, &AMainCharacter::MagicOn);
}

void AMainCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && (!IsRolling) && (!IsAttack) && (!IsBlock) && ActivateSkill == false && IsHealing == false)
	{
		const FRotator Roation = Controller->GetControlRotation(); // ��ü ȸ�� �� ����
		const FRotator YawRoation = FRotator(0, Roation.Yaw, 0); // Yaw ȸ��������

		const FVector Direction = FRotationMatrix(YawRoation).GetUnitAxis(EAxis::X); // ���� ����
		AddMovementInput(Direction, Value); // �������� �ӵ� ��ŭ ������ ��
	}
}

// MoveForward�� ��� ����
void AMainCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && (!IsRolling) && (!IsAttack) && (!IsBlock) && ActivateSkill == false && IsHealing == false)
	{
		const FRotator Roation = Controller->GetControlRotation();
		const FRotator YawRoation = FRotator(0, Roation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRoation).GetUnitAxis(EAxis::Y); // X���� �ƴ� Y������
		AddMovementInput(Direction, Value);
	}
}

// �뽬 �Լ�
void AMainCharacter::Dash()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed; // ȣ��Ǹ� ĳ���͹����Ʈ�� �ִ�ȱ� �ӵ��� ������
	IsDash = true;
}

// �뽬 ���� �Լ�
void AMainCharacter::DashStop()
{
	GetCharacterMovement()->MaxWalkSpeed = LowSpeed; // ȣ��Ǹ� ĳ���͹����Ʈ�� �ִ�ȱ� �ӵ��� ������
	IsDash = false;
}

// ������ �Լ�
void  AMainCharacter::Rolling()
{
	if ((!IsRolling) && SP - RollingSP >= 0) // ���� ������ ������ �Ǵ� �ƴ϶�� ����
	{
		IsRolling = true; // ���� ������ ������ ����
		SP -= RollingSP;
		auto AnimInstance = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance()); // AnimInstance ���� ����
		if (nullptr == AnimInstance) { return; } // �ִϸ��̼��� ���� �Ǿ� ���� ������ retrun

		AnimInstance->PlayRollingMontage(); // MainAnimInstance�� �ִ� PlayRollingMintage ȣ��
	}
}

// ������ ���� �Լ� (�ζ��� �Լ��� �����ϴ� �� ���)
void AMainCharacter::RollingEnd()
{
	IsRolling = false;
}

//���� ��� ���� �Լ�(�Ѽհ� +1, ���� +1, ��� +3 ���� ���� �Ͽ� Mode ���� 2 �̸� ��&���� ���, 3�̸� ��� ���)
void AMainCharacter::SetMode(int ModeNum)
{
	Mode += ModeNum;
}

//���콺 ��Ŭ�� �� ȣ��Ǵ� �Լ�
void AMainCharacter::LMBdawn()
{
	bLMBdawn = true; // ��Ŭ�� �Ѱ����� ����

	if (IsAttack == false && ActivateSkill == false && IsHealing == false && !IsBlock) // ���� ��X ��ų ���X, ���� ���X �϶�  ���� �Լ� ȣ��
	{
		Attack(); 
	}
	else if (IsAttack == true) // �̹� �������̸� �޺� ������ �ҷ��°����� �Ǵ�, ���� ����
	{
		AttackButtonWhenAttack = true;
	}
}

// ���� �Լ�
void AMainCharacter::Attack()
{
	bLMBdawn = true; 

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	IsAttack = true; // ���� ������ ���� ����

	const char* combolist[] = { "Attack1", "Attack2", "Attack3" }; // �ִ� ��Ÿ�� ������ �迭�� ����

	if ( Mode == 2) // ��&���� ���¶��׿� �´� �ִϸ��̼� ���
	{
		SetActorRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
		if (!AnimInstance->Montage_IsPlaying(SwordAndShiledMontage)) // ��Ÿ�ְ� ��� ���� �ƴϸ�
		{
			AnimInstance->Montage_Play(SwordAndShiledMontage,1.2f); //��Ÿ�� ���(1�� �޺�)
		}
		else if (AnimInstance->Montage_IsPlaying(SwordAndShiledMontage)) // �̹� ��Ÿ�ְ� ��� ���̸� -> ���� ������ ���� �Ǵ� �޺� ���� ���
		{
			AnimInstance->Montage_Play(SwordAndShiledMontage); //��Ÿ�� ���(1�� �޺�)
			AnimInstance->Montage_JumpToSection(FName(combolist[ComboNum]), SwordAndShiledMontage); // 2 or 3��°(combolist[ComboNum])�������� �̵�
		}
	}
	else if (Mode == 3) // ��� ���¿� �´� �ִϸ��̼� ���
	{
		SetActorRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
		if (!(AnimInstance->Montage_IsPlaying(GreatSwordMontage)))
		{
			AnimInstance->Montage_Play(GreatSwordMontage);
		}
		else if (AnimInstance->Montage_IsPlaying(GreatSwordMontage))
		{
			AnimInstance->Montage_Play(GreatSwordMontage); // 0.65���� �ӵ��� ���
			AnimInstance->Montage_JumpToSection(FName(combolist[ComboNum]), GreatSwordMontage);
		}
	}
	else
	{
		IsAttack = false;
	}
}

void AMainCharacter::AttackEnd() // ������ ���� �Ǿ����� ������ ���¸� false��
{
	IsAttack = false;
}

void AMainCharacter::BlockStart() //���콺 ��Ŭ����  ȣ��, ���� ��&���� ����̸�, IsBlock �� true�� ����, ��� ���Լ� ȣ��
{
	if (Mode == 2 && SP > 0)
	{
		IsBlock = true;
		Block();
	}
	else
	{
		return;
	}
	
}


void AMainCharacter::Block() // ��� �� �Լ�, ���� ��� ���� �̸�, �ִϸ��̼��� ����ϰ�, ���¹̳��� ����, �� �Լ��� Ÿ�̸Ӹ� �̿��Ͽ� 0.75�ʸ��� ������ üũ �� �� ȣ��
{
	if (IsBlock && SP > 0)
	{
		SetActorRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(BlockMontage);
		SP -= 3.5f;
		bIsRecovery = false;
		GetWorld()->GetTimerManager().SetTimer(timer, this, &AMainCharacter::Block, 0.75f, false);
	}
}

void AMainCharacter::BlockEnd()
{
	if (Mode == 2)
	{
		IsBlock = false;
		auto AnimInstance = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());
		//AnimInstance->Montage_Stop(0.0f, BlockMontage);
		AnimInstance->PlayBlockEndMontage();
		bIsRecovery = true;
	}
	
}

//�޺� üũ�� �Լ�
void AMainCharacter::CheackCombo()
{
	if (ComboNum >= 2) // �޺� ��ȣ�� 2������ ũ�� = ������ �޺�
	{
		ComboNum = 0; // �޺� �ʱ�ȭ
	}
	if (AttackButtonWhenAttack == true) // ���� �߿� ���콺 Ŭ�� ��(�޺� ��� ��)
	{
		ComboNum += 1; //�޺� ��ȣ ����
		AttackButtonWhenAttack = false; //���� �� ���콺 Ŭ�� false(���� �޺� ���� Ȯ���� ����)
		Attack(); // ���� �Լ� ȣ��
	}
}

// Q��ų �۵� �Լ�
void AMainCharacter::QSkillActive()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!ActivateSkill && QSkillMontage != nullptr && IsHealing == false && !IsBlock) // ���� ��X, Q��ų�� ���� O, ���� ���X
	{
		ActivateSkill = true; //���� ��ų�� ���·� ����
		AnimInstance->Montage_Play(QSkillMontage); // ��ų �ִϸ��̼� ���
	}
	else
	{
		return;
	}
}

// Q��ų�� ����
void AMainCharacter::ESkillActive()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!ActivateSkill && ESkillMontage != nullptr && IsHealing == false)
	{
		ActivateSkill = true;
		AnimInstance->Montage_Play(ESkillMontage);
	}
	else
	{
		return;
	}
}

// R��ư�� ������ ������ ���ô� ����� ����
void AMainCharacter::UsePortion()
{
	if (!(IsHealing))
	{
		if (PortionNum > 0)
		{
			IsHealing = true;
			auto AnimInstance = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());
			AnimInstance->PlayPortionMontage();
		}
	}
}

// ������ ���ô� ����� ������, ������ ���� �ٰ�, HP�� ȸ�� ��
void AMainCharacter::DecreasePortion()
{
	PortionNum -= 1;
	if (HP + Healing >= MaxHP)
	{
		HP = MaxHP;
	}
	else
	{
		HP += Healing;
	}
	IsHealing = false;
}

// ���׹̳� �ڵ� ȸ�� �Լ�(�������Ʈ���� ���� �ð����� ȣ���)
void AMainCharacter::Recovery()
{
	if (bIsRecovery)
	{
		if (SP < MaxSP)
		{
			SP += RecoverySP;

			if (SP > MaxSP)
			{
				SP = MaxSP;
			}
		}
	}
	
}
// ����ĳ������ ���� ����(��&���� ��� or ��˸��)
void AMainCharacter::SettingWeapon(AWeapon* Weapon)
{
	MainWeapon = Weapon;
}

// ���� �ߵ� �Լ�
void AMainCharacter::MagicOn()
{
	if (::IsValid(MainWeapon))	//����ĳ������ ���Ⱑ ���� �Ǿ� ������ -> ��&���� ��� or ��˸��
	{
		auto AnimInstance = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());

		if (Mode == 2 && !(MainWeapon->bIsMagic)) //��&���� ��� �̰� ����������� �ƴҶ�
		{
			AnimInstance->PlayOnHandCastMontage(); //�ִ� �ν��Ͻ����� �ִϸ��̼� ���
			MainWeapon->MagicActivate();
			
		}
		else if (Mode > 2 && !(MainWeapon->bIsMagic)) //��� ��� �̰� ����������� �ƴҶ�
		{
			AnimInstance->PlayGreatSwordCastMontage(); //�ִ� �ν��Ͻ����� �ִϸ��̼� ���
			MainWeapon->MagicActivate();
		}
	}
	else
	{
		return;
	}
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}