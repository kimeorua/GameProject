// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyController.h"
#include "EnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/BoxComponent.h"
#include "MainCharacter.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHP = 100.f; //생명력 초기화
	HP = MaxHP;

	//적 캐릭터의 회전을 좀더 자연스럽게 하기위한 초기화
	bUseControllerRotationYaw = false; 
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);

	AxeCollision1 = CreateDefaultSubobject<UBoxComponent>(TEXT("AxeCollision1"));

	AxeCollision2 = CreateDefaultSubobject<UBoxComponent>(TEXT("AxeCollision2"));

	IsAttacking = false;
	EnemyAnim = nullptr;
	CombetMain = nullptr;

	Axe1Damage = 6.f;
	Axe2Damage = 10.f;
}

void AEnemy::PostInitializeComponents()
{
	//애니메이션 변수 초기화 
	Super::PostInitializeComponents();
	EnemyAnim = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	// (OnMontageEnded)몽타주가 종료될 때 호출되는 델리게이트를 이용하여 OnAttackMontageEnded함수를 몽타주 종료시 호출한다.
	EnemyAnim->OnMontageEnded.AddDynamic(this, &AEnemy::OnAttackMontageEnded);
}

void AEnemy::Axe1OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (IsValid(Main))
		{
			UGameplayStatics::ApplyDamage(Main, Axe1Damage, this->GetController(), this, NULL); // 데미지 주는 함수
		}
		else { return; }
	}
	else { return; }
	//UE_LOG(LogTemp, Warning, TEXT("OtherActor:: %s" ), *OtherActor->GetName());
}

void AEnemy::Axe2OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (IsValid(Main))
		{ 
			UGameplayStatics::ApplyDamage(Main, Axe2Damage, this->GetController(), this, NULL); // 데미지 주는 함수
		}
		else { return; }
	}
	else { return; }
}

// 데미지를 받을시 호출 되는 함수
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); 
	IsAttacking = false; //공격 중이면 공격중을 false로 설정(공격 초기화)
	if (HP > 0)
	{
		if (HP - DamageAmount <= 0)
		{
			HP = 0;
			Die(DamageCauser);
			if (IsValid(CombetMain))
			{
				CombetMain->KillNum += 1;
			}
		}
		else
		{
			HP -= DamageAmount;
			Hit();
		}
	}
	return Damage;
}

void AEnemy::Hit()
{
	EnemyAnim->PlayHitMontage();
	AxeCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AxeCollision2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Stun()
{
	EnemyAnim->PlayStunMontage();
	AxeCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AxeCollision2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Die(AActor* DamageCauser)
{
	EnemyAnim->PlayDieMontage();
	AEnemyController* EnemyController = Cast<AEnemyController>(GetController());
	EnemyController->BehaviorTreeComponent->StopTree();

	FTimerHandle WaitHandle;
	float WaitTime = 1.0f; //시간을 설정하고
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AxeCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AxeCollision2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Destroy();
		}), WaitTime, false); //반복도 여기서 추가 변수를 선언해 설정가능
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AxeCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AxeCollision1->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SokcetName1);
	AxeCollision2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AxeCollision2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SokcetName2);

	AxeCollision1->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::Axe1OverlapBegin);
	AxeCollision2->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::Axe2OverlapBegin);
}

void AEnemy::Attack(int num)
{
	IsAttacking = true;
	EnemyAnim->PlayAttackMontage(num);
}

void AEnemy::RangeAttack()
{
	IsAttacking = true;
	EnemyAnim->PlayRangeAttackMontage();
}

void AEnemy::Axe1CollisionStart()
{
	AxeCollision1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::Axe1CollisionEnd()
{
	AxeCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Axe2CollisionStart()
{
	AxeCollision2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::Ax2CollisionEnd()
{
	AxeCollision2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast(); //BTTask_Attack에서 선언한 공격 종료 함수가 호출되도록 델리게이트를 이용하여 구축
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}