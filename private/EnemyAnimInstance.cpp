// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{ 
	// �ʱ�ȭ
	Speed = 0.0f;
	AttackMontageWeak = nullptr;
	AttackMontageStrong = nullptr;
	RangeAttackMontageStrong = nullptr;
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	auto Pawn = TryGetPawnOwner(); // Pawn ������ �÷��̾� ĳ���� ����
	if (IsValid(Pawn)) // ��ȿ �ϸ�
	{
		auto Enemy = Cast<AEnemy>(Pawn);			 // Main ������ ����
		Speed = Enemy->GetVelocity().Size();			// ĳ������ �ӵ��� ���� speed ����ȭ
	}
}

void UEnemyAnimInstance::PlayAttackMontage(int num)
{
	if (num == 1)
	{
		Montage_Play(AttackMontageWeak, 1.3f);
	}
	else if (num == 2)
	{
		Montage_Play(AttackMontageStrong, 1.3f);
	}
	
}

void UEnemyAnimInstance::PlayRangeAttackMontage()
{
	Montage_Play(RangeAttackMontageStrong, 1.3f);
}

void UEnemyAnimInstance::PlayHitMontage()
{
	if (!Montage_IsPlaying(HitMontage))
	{
		Montage_Play(HitMontage);
	}
}

void UEnemyAnimInstance::PlayDieMontage()
{
	if (!Montage_IsPlaying(DieMontage))
	{
		Montage_Play(DieMontage);
	}
}
