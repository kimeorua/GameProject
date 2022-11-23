// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{ 
	// 초기화
	Speed = 0.0f;
	AttackMontageWeak = nullptr;
	AttackMontageStrong = nullptr;
	RangeAttackMontageStrong = nullptr;
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	auto Pawn = TryGetPawnOwner(); // Pawn 변수에 플레이어 캐릭터 저장
	if (IsValid(Pawn)) // 유효 하면
	{
		auto Enemy = Cast<AEnemy>(Pawn);			 // Main 변수에 저장
		Speed = Enemy->GetVelocity().Size();			// 캐릭터의 속도와 변수 speed 동기화
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
