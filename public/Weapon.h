// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class CPP_MAGNUSBELLATROR_API AWeapon : public AActor
{
	GENERATED_BODY()
	
private: 
	// 스켈레탈 매쉬
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Mesh;

	// 장착 가능 반경
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* Collison;

	// 화살표(앞뒤 방향 확인)
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* Arrow;

	//가까이 가면 보일 택스트
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* Text;

	//공격 충돌 판정
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CombetCollison;

public: 

	// 플레이어 캐릭터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main")
	class AMainCharacter* Main;

	// 장착 가능한지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main")
	bool EquipAble;

	// 장착될 소켓 이름 -> 디테일 탭에서 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main")
	FName SocketName;

	// 모드 셋팅용 숫자 -> 디테일 탭에서 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	int ModeNum;

	// 장착 시 출력될 효과음 -> 디테일 탭에서 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	USoundBase* EquipSound;

	// Q,E 스킬 애니메이션 -> 디테일 탭에서 설정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	UAnimMontage* QSkillMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	UAnimMontage* ESkillMontage;

	// 마법 스킬 사용시 무기에 생기는 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	UParticleSystemComponent* MagicParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main")
	bool bIsMagic;

	FName CombetCollisionName = TEXT("Root");

	//스킬 번호
	int SkillNum;

	//기본데미지->에디터에서 설정 가능
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	float UpDamage;

public:	
	// Sets default values for this actor's properties
	AWeapon(); // 생성자

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 오버랩 시
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//오버랩 종료 시
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//장착 함수
	UFUNCTION(BlueprintCallable)
	void Equip();

	// 마법 발동
	void MagicActivate();
	// 마법 해제
	void MagicDeactivate();

	UFUNCTION()
	void CombetOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void StartCollision();

	UFUNCTION(BlueprintCallable)
	void EndCollision();
};
