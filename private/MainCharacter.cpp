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
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Sheild.h"

// Sets default values
// 변수 초기화 및 컴포넌트 생성
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 변수 초기화//
	MaxSpeed = 800.0f;
	LowSpeed = 400.0f;
	MoveDistance = 750.0f;

	ComboNum = 0;
	Mode = 0;

	PortionNum = 3;

	MaxHP = 100.0f;
	MaxSP = 100.0f;

	HP = MaxHP;
	SP = MaxSP;

	Healing = 25.0f;
	RollingSP = 10.0f;

	RecoverySP = 0.5f;

	IsRolling = false;
	IsDash = false;
	bLMBdawn = false;
	IsAttack = false;
	AttackButtonWhenAttack = false;
	IsBlock = false;
	ActivateSkill = false;

	SwordAndShiledMontage = nullptr;
	GreatSwordMontage = nullptr;
	BlockMontage = nullptr;

	QSkillMontage = nullptr;
	ESkillMontage = nullptr;
	MainWeapon = nullptr;

	IsHealing = false;
	bIsParrying = false;
	CombetEnemy = nullptr;

	KillNum = 0;

	// 컴포넌트 //
	//
	// 캡슐 컴포넌트 크기 설정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));//스프링 암 컴포넌트 생성
	CameraBoom->SetupAttachment(RootComponent); // RootComponent에 부착
	CameraBoom->TargetArmLength = 400.0f; //팔길이 400으로 설정
	CameraBoom->bUsePawnControlRotation = true; // 컨트롤러 회전 사용
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")); // 카메라 컴포넌트 생성
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // 카메라 부착(스프링 암)
	FollowCamera->bUsePawnControlRotation = false;		// 컨트롤 회전 사용 X
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	//축 입력//
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);

	//액션//

	//대쉬
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMainCharacter::Dash);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &AMainCharacter::DashStop);
	//공격
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainCharacter::LMBdawn);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AMainCharacter::LMBup);
	//방어
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AMainCharacter::BlockStart);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &AMainCharacter::BlockEnd);
	//스킬사용
	PlayerInputComponent->BindAction("QSkill", IE_Pressed, this, &AMainCharacter::QSkillActive);
	PlayerInputComponent->BindAction("ESkill", IE_Pressed, this, &AMainCharacter::ESkillActive);

	//포션 마시기
	PlayerInputComponent->BindAction("Drinking", IE_Pressed, this, &AMainCharacter::UsePortion);

	//마법 사용
	PlayerInputComponent->BindAction("Magic", IE_Pressed, this, &AMainCharacter::MagicOn);
}

void AMainCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && (!IsRolling) && (!IsAttack) && (!IsBlock) && (!ActivateSkill) && (!IsHealing))
	{
		const FRotator Roation = Controller->GetControlRotation(); // 전체 회전 값 구함
		const FRotator YawRoation = FRotator(0, Roation.Yaw, 0); // Yaw 회전값구함

		const FVector Direction = FRotationMatrix(YawRoation).GetUnitAxis(EAxis::X); // 방향 추출
		AddMovementInput(Direction, Value); // 방향으로 속도 만큼 움직임 줌
	}
}

// MoveForward와 방식 동일
void AMainCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && (!IsRolling) && (!IsAttack) && (!IsBlock) && (!ActivateSkill) && (!IsHealing))
	{
		const FRotator Roation = Controller->GetControlRotation();
		const FRotator YawRoation = FRotator(0, Roation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRoation).GetUnitAxis(EAxis::Y); // X축이 아닌 Y축으로
		AddMovementInput(Direction, Value);
	}
}

// 대쉬 함수
void AMainCharacter::Dash()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed; // 호출되면 캐릭터무브먼트의 최대걷기 속도를 수정함
	IsDash = true;
}

// 대쉬 종료 함수
void AMainCharacter::DashStop()
{
	GetCharacterMovement()->MaxWalkSpeed = LowSpeed; // 호출되면 캐릭터무브먼트의 최대걷기 속도를 수정함
	IsDash = false;
}

// 구르기 함수
void  AMainCharacter::Rolling()
{
	if ((!IsRolling) && (!IsAttack) && (!IsBlock) && (!ActivateSkill) && (!IsHealing) && SP - RollingSP >= 0) // 현제 구르기 중인지 판단 아니라면 실행
	{
		IsRolling = true; // 현제 구르기 중으로 변경
		SP -= RollingSP;
		auto AnimInstance = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance()); // AnimInstance 변수 생성
		if (nullptr == AnimInstance) { return; } // 애니메이션이 지정 되어 있지 않으면 retrun
		AnimInstance->PlayRollingMontage(); // MainAnimInstance에 있는 PlayRollingMintage 호출
	}
}

// 구르기 종료 함수 (인라인 함수로 변경하는 것 고려)
void AMainCharacter::RollingEnd()
{
	IsRolling = false;
}

//현제 모드 설정 함수(한손검 +1, 방패 +1, 대검 +3 으로 설정 하여 Mode 값이 2 이면 검&방패 사용, 3이면 대검 사용)
void AMainCharacter::SetMode(int ModeNum)
{
	Mode += ModeNum;
}

//마우스 좌클릭 시 호출되는 함수
void AMainCharacter::LMBdawn()
{
	bLMBdawn = true; // 좌클릭 한것으로 변경

	if ((!IsRolling) && (!IsAttack) && (!IsBlock) && (!ActivateSkill) && (!IsHealing)) // 공격 중X 스킬 사용X, 포션 사용X 일때  공격 함수 호출
	{
		Attack();
	}
	else if (IsAttack == true) // 이미 공격중이면 콤보 공격을 할려는것으로 판단, 변수 설정
	{
		AttackButtonWhenAttack = true;
	}
}

// 공격 함수
void AMainCharacter::Attack()
{
	bLMBdawn = true; 

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	IsAttack = true; // 공격 중으로 상태 변경

	const char* combolist[] = { "Attack1", "Attack2", "Attack3" }; // 애님 몽타주 섹션을 배열에 저장

	if ( Mode == 2) // 검&방패 상태라면그에 맞는 애니메이션 출력
	{
		bIsRecovery = false;
		SetActorRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
		if (!AnimInstance->Montage_IsPlaying(SwordAndShiledMontage)) // 몽타주가 재생 중이 아니면
		{
			AnimInstance->Montage_Play(SwordAndShiledMontage,1.2f); //몽타주 재생(1번 콤보)
		}
		else if (AnimInstance->Montage_IsPlaying(SwordAndShiledMontage)) // 이미 몽타주가 재생 중이면 -> 현제 공격중 으로 판단 콤보 공격 출력
		{
			AnimInstance->Montage_Play(SwordAndShiledMontage, 1.2f); //몽타주 재생(1번 콤보)
			AnimInstance->Montage_JumpToSection(FName(combolist[ComboNum]), SwordAndShiledMontage); // 2 or 3번째(combolist[ComboNum])섹션으로 이동
		}
	}
	else if (Mode == 3) // 대검 상태에 맞는 애니메이션 출력
	{
		bIsRecovery = false;
		SetActorRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
		if (!(AnimInstance->Montage_IsPlaying(GreatSwordMontage)))
		{
			AnimInstance->Montage_Play(GreatSwordMontage);
		}
		else if (AnimInstance->Montage_IsPlaying(GreatSwordMontage))
		{
			AnimInstance->Montage_Play(GreatSwordMontage); // 0.65배의 속도로 재생
			AnimInstance->Montage_JumpToSection(FName(combolist[ComboNum]), GreatSwordMontage);
		}
	}
	else
	{
		IsAttack = false;
	}
}

void AMainCharacter::AttackEnd() // 공격이 종료 되었으면 공격중 상태를 false로
{
	IsAttack = false;
	bIsRecovery = true;
}

void AMainCharacter::BlockStart() //마우스 우클릭시  호출, 현제 검&방패 모드이면, IsBlock 을 true로 설정, 방어 중함수 호출
{
	if (Mode == 2 && SP > 0 && (!IsRolling) && (!IsAttack) && (!ActivateSkill) && (!IsHealing))
	{
		IsBlock = true;
		Block();
		ParryingStart();

		FTimerHandle BlockWaitHandle;
		float WaitTime = 0.25f; //시간을 설정하고
		GetWorld()->GetTimerManager().SetTimer(BlockWaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				ParryingEnd();
			}), WaitTime, false); //반복도 여기서 추가 변수를 선언해 설정가
	}
	else
	{
		return;
	}
	
}


void AMainCharacter::Block() // 방어 중 함수, 현제 방어 상태 이면, 애니메이션을 재생하고, 스태미나를 감소, 이 함수를 타이머를 이용하여 0.75초마다 방어상태 체크 및 재 호출
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
		AnimInstance->PlayBlockEndMontage();
		bIsRecovery = true;
	}
	
}

//콤보 체크용 함수
void AMainCharacter::CheackCombo()
{
	if (ComboNum >= 2) // 콤보 번호가 2번보다 크면 = 마지막 콤보
	{
		ComboNum = 0; // 콤보 초기화
	}
	if (AttackButtonWhenAttack == true) // 공격 중에 마우스 클릭 시(콤보 사용 시)
	{
		ComboNum += 1; //콤보 번호 증가
		AttackButtonWhenAttack = false; //공격 중 마우스 클릭 false(다음 콤보 여부 확인을 위해)
		Attack(); // 공격 함수 호출
	}
}

// Q스킬 작동 함수 스킬 사용시 해당하는 스킬 번호 지정
void AMainCharacter::QSkillActive()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if ((!IsRolling) && (!IsAttack) && (!IsBlock) && (!ActivateSkill) && (!IsHealing) && QSkillMontage != nullptr) // 공격 중X, Q스킬이 설정 O, 포션 사용X
	{
		ActivateSkill = true; //현제 스킬중 상태로 변경
		AnimInstance->Montage_Play(QSkillMontage); // 스킬 애니메이션 재생
		MainWeapon->SkillNum = 1;
	}
	else
	{
		return;
	}
}

// Q스킬과 동일, 스킬 사용시 해당하는 스킬 번호 지정
void AMainCharacter::ESkillActive()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if ((!IsRolling) && (!IsAttack) && (!IsBlock) && (!ActivateSkill) && (!IsHealing) && ESkillMontage != nullptr)
	{
		ActivateSkill = true;
		AnimInstance->Montage_Play(ESkillMontage);
		MainWeapon->SkillNum = 2;
	}
	else
	{
		return;
	}
}

// R버튼을 누르면 포션을 마시는 모션이 나옴
void AMainCharacter::UsePortion()
{
	if ((!IsRolling) && (!IsAttack) && (!IsBlock) && (!ActivateSkill) && (!IsHealing))
	{
		if (PortionNum > 0)
		{
			IsHealing = true;
			auto AnimInstance = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());
			AnimInstance->PlayPortionMontage();
		}
	}
}

// 포션을 마시는 모션이 끝나면, 포션의 수가 줄고, HP가 회복 됨
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

// 스테미나 자동 회복 함수(블루프린트에서 일정 시간마다 호출됨)
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
// 매인캐릭터의 무기 설정(검&방패 모드 or 대검모드)
void AMainCharacter::SettingWeapon(AWeapon* Weapon)
{
	MainWeapon = Weapon;
}

void AMainCharacter::SettingSheild(ASheild* Sheild)
{
	MainSheild = Sheild;
}

// 마법 발동 함수
void AMainCharacter::MagicOn()
{
	if (::IsValid(MainWeapon) && (!IsRolling) && (!IsAttack) && (!IsBlock) && (!ActivateSkill) && (!IsHealing))	//매인캐릭터의 무기가 설정 되어 있으면 -> 검&방패 모드 or 대검모드
	{
		auto AnimInstance = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());

		if (Mode == 2 && !(MainWeapon->bIsMagic)) //검&방패 모드 이고 마법사용중이 아닐때
		{
			AnimInstance->PlayOnHandCastMontage(); //애님 인스턴스에서 애니매이션 출력
			MainWeapon->MagicActivate();
			
		}
		else if (Mode > 2 && !(MainWeapon->bIsMagic)) //대검 모드 이고 마법사용중이 아닐때
		{
			AnimInstance->PlayGreatSwordCastMontage(); //애님 인스턴스에서 애니매이션 출력
			MainWeapon->MagicActivate();
		}
	}
	else
	{
		return;
	}
}

//데미지 받을 시 호출 하는 함수
float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (!IsRolling)
	{
		if (IsBlock && MainSheild->bUseBlock)
		{
			if (bIsParrying) //패링중
			{
				AEnemy* Enemy = Cast<AEnemy>(DamageCauser);
				Enemy->Stun();
				MainSheild->PlayParringSound();
			}
			else
			{
				if (SP - DamageAmount <= 0)
				{
					SP = 0;
					Stun();
				}
				else
				{
					SP -= DamageAmount * 2.5;
					MainSheild->PlayBloackSound();
				}
			}
		}
		else if (!(IsBlock) || !(MainSheild->bUseBlock))
		{
			if (HP - DamageAmount <= 0)
			{
				HP = 0;
				Die();
			}
			else
			{
				HP -= DamageAmount;
				Hit();
			}
		}
	}
	return Damage;
}

void AMainCharacter::Hit()
{
	if (!ActivateSkill)
	{
		auto AnimInstance = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());
		AnimInstance->PlayHitMontage();
		DisableInput(UGameplayStatics::GetPlayerController(this, 0));
		IsAttack = false;
		IsBlock = false;
		bIsRecovery = true;
	}
	else
	{
		return;
	}
}

void AMainCharacter::HitEnd()
{
	EnableInput(UGameplayStatics::GetPlayerController(this, 0));
}

void AMainCharacter::Die()
{
	auto AnimInstance = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->PlayDieMontage();
	DisableInput(UGameplayStatics::GetPlayerController(this, 0));
	if (IsValid(MainWeapon))
	{
		MainWeapon->EndCollision();
	}
	if (IsValid(MainSheild))
	{
		MainSheild->EndCollision();
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle WaitHandle;
	float WaitTime = 2.3f; //시간을 설정하고
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (IsValid(MainWeapon)) 
			{
				MainWeapon->Destroy();
			}
			if (IsValid(MainSheild))
			{
				MainSheild->Destroy();
			}
			Destroy();
		}), WaitTime, false); //반복도 여기서 추가 변수를 선언해 설정가능
}

//스태미나 소진시 가드하면 스턴
void AMainCharacter::Stun()
{
	IsBlock = false;
	MainSheild->bUseBlock = false;
	bIsRecovery = true;
	auto AnimInstance = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->playStunMontage();
	DisableInput(UGameplayStatics::GetPlayerController(this, 0));
}

void AMainCharacter::StunEnd()
{
	EnableInput(UGameplayStatics::GetPlayerController(this, 0));
}

//패링 가능
void AMainCharacter::ParryingStart()
{
	bIsParrying = true;
}
//패링 불가능
void AMainCharacter::ParryingEnd()
{
	bIsParrying = false;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	bIsRecovery = true;
	CombetEnemy = nullptr;
	IsRolling = false;
	IsDash = false;
	bLMBdawn = false;
	IsAttack = false;
	AttackButtonWhenAttack = false;
	IsBlock = false;
	ActivateSkill = false;
	
	MainWeapon = nullptr;
	MainSheild = nullptr;
	KillNum = 0;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}