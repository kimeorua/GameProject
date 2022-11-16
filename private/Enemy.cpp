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

	HP = 100.0; //����� �ʱ�ȭ

	//�� ĳ������ ȸ���� ���� �ڿ������� �ϱ����� �ʱ�ȭ
	bUseControllerRotationYaw = false; 
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
}

void AEnemy::PostInitializeComponents()
{
	//�ִϸ��̼� ���� �ʱ�ȭ 
	Super::PostInitializeComponents();
	EnemyAnim = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	// (OnMontageEnded)��Ÿ�ְ� ����� �� ȣ��Ǵ� ��������Ʈ�� �̿��Ͽ� OnAttackMontageEnded�Լ��� ��Ÿ�� ����� ȣ���Ѵ�.
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
	OnAttackEnd.Broadcast(); //BTTask_Attack���� ������ ���� ���� �Լ��� ȣ��ǵ��� ��������Ʈ�� �̿��Ͽ� ����
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



