// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyController.h"
#include "EnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HP = 100.0; //생명력 초기화

	//적 캐릭터의 회전을 좀더 자연스럽게 하기위한 초기화
	bUseControllerRotationYaw = false; 
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
}

void AEnemy::PostInitializeComponents()
{
	//애니메이션 변수 초기화 
	Super::PostInitializeComponents();
	EnemyAnim = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	// (OnMontageEnded)몽타주가 종료될 때 호출되는 델리게이트를 이용하여 OnAttackMontageEnded함수를 몽타주 종료시 호출한다.
	EnemyAnim->OnMontageEnded.AddDynamic(this, &AEnemy::OnAttackMontageEnded);
}


// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
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



