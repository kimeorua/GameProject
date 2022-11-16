// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "MainCharacter.h"

UMainAnimInstance::UMainAnimInstance()
{
	//변수 초기화
	Speed = 0.0f; 
	mode = 0;

	RollingMontage = nullptr;
	PortionMontage = nullptr;

	OnHandCastMontage = nullptr;
	GreatSwordCastMontage = nullptr;
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto Pawn = TryGetPawnOwner(); // Pawn 변수에 플레이어 캐릭터 저장

	if (::IsValid(Pawn)) // 유효 하면
	{
		auto Main = Cast<AMainCharacter>(Pawn); // Main 변수에 저장
		Speed = Main->GetVelocity().Size(); // 캐릭터의 속도와 변수 speed 동기화
		mode = Main->Mode; // 캐릭터의 모드와 변수 Mode 동기화
	}
}

//회피 재생 함수
void UMainAnimInstance::PlayRollingMontage()
{
	if (!Montage_IsPlaying(RollingMontage)) // 회피 애니메이션 재생중이 아니면(회피 중이 아니면)
	{
		Montage_Play(RollingMontage, 1.0f); // 회피 애니메이션 재생
	}
}

// 포션 사용 재생 함수
void UMainAnimInstance::PlayPortionMontage()
{
	if (!Montage_IsPlaying(PortionMontage)) // 포션 사용 애니메이션 재생중이 아니면(포션 사용 중이 아니면)
	{
		Montage_Play(PortionMontage, 1.5f); // 포션 사용애니메이션 재생
	}
}

//위와 동일 방식
void UMainAnimInstance::PlayOnHandCastMontage()
{
	if (!Montage_IsPlaying(OnHandCastMontage))
	{
		Montage_Play(OnHandCastMontage);
	}
}

//위와 동일 방식
void UMainAnimInstance::PlayGreatSwordCastMontage()
{
	if (!Montage_IsPlaying(GreatSwordCastMontage))
	{
		Montage_Play(GreatSwordCastMontage);
	}
}

void UMainAnimInstance::PlayBlockEndMontage()
{
	if (!Montage_IsPlaying(BlockEndMontage))
	{
		Montage_Play(BlockEndMontage);
	}
}

void UMainAnimInstance::AnimNotify_UsePortionEnd()
{
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		auto Main = Cast<AMainCharacter>(Pawn);
		Main->DecreasePortion();
	}
}
