// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

UCLASS()
class CPP_MAGNUSBELLATROR_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	//변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	float Speed; //캐릭터의 속도

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	int mode; // 캐릭터의 상태(일반, 검&방패, 대검)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move")
	UAnimMontage* RollingMontage; //회피 애니메이션-> 애니메이션 블루프린트에서 설정 함

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Healing")
	UAnimMontage* PortionMontage; //포션 애니메이션-> 애니메이션 블루프린트에서 설정 함

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* OnHandCastMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* GreatSwordCastMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* BlockEndMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* DieMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	UAnimMontage* StunMontage;

public:
	UMainAnimInstance(); // 생성자

	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //애니메이션 업데이트 함수

	void PlayRollingMontage(); // 회피 애니메이션 재생

	void PlayPortionMontage(); //포션 사용 애니메이션 재생

	void PlayOnHandCastMontage(); //검&방패 마법 애니매이션 재생
	void PlayGreatSwordCastMontage();  //대검 마법 애니매이션 재생
	void PlayBlockEndMontage();

	void PlayHitMontage();
	void PlayDieMontage();
	void playStunMontage();

	UFUNCTION()
	void AnimNotify_UsePortionEnd();
};
