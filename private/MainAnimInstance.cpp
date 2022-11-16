// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "MainCharacter.h"

UMainAnimInstance::UMainAnimInstance()
{
	//���� �ʱ�ȭ
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
	auto Pawn = TryGetPawnOwner(); // Pawn ������ �÷��̾� ĳ���� ����

	if (::IsValid(Pawn)) // ��ȿ �ϸ�
	{
		auto Main = Cast<AMainCharacter>(Pawn); // Main ������ ����
		Speed = Main->GetVelocity().Size(); // ĳ������ �ӵ��� ���� speed ����ȭ
		mode = Main->Mode; // ĳ������ ���� ���� Mode ����ȭ
	}
}

//ȸ�� ��� �Լ�
void UMainAnimInstance::PlayRollingMontage()
{
	if (!Montage_IsPlaying(RollingMontage)) // ȸ�� �ִϸ��̼� ������� �ƴϸ�(ȸ�� ���� �ƴϸ�)
	{
		Montage_Play(RollingMontage, 1.0f); // ȸ�� �ִϸ��̼� ���
	}
}

// ���� ��� ��� �Լ�
void UMainAnimInstance::PlayPortionMontage()
{
	if (!Montage_IsPlaying(PortionMontage)) // ���� ��� �ִϸ��̼� ������� �ƴϸ�(���� ��� ���� �ƴϸ�)
	{
		Montage_Play(PortionMontage, 1.5f); // ���� ���ִϸ��̼� ���
	}
}

//���� ���� ���
void UMainAnimInstance::PlayOnHandCastMontage()
{
	if (!Montage_IsPlaying(OnHandCastMontage))
	{
		Montage_Play(OnHandCastMontage);
	}
}

//���� ���� ���
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
