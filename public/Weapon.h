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
	// ���̷�Ż �Ž�
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Mesh;

	// ���� ���� �ݰ�
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* Collison;

	// ȭ��ǥ(�յ� ���� Ȯ��)
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* Arrow;

	//������ ���� ���� �ý�Ʈ
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* Text;

	//���� �浹 ����
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CombetCollison;

public: 

	// �÷��̾� ĳ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main")
	class AMainCharacter* Main;

	// ���� �������� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main")
	bool EquipAble;

	// ������ ���� �̸� -> ������ �ǿ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main")
	FName SocketName;

	// ��� ���ÿ� ���� -> ������ �ǿ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	int ModeNum;

	// ���� �� ��µ� ȿ���� -> ������ �ǿ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	USoundBase* EquipSound;

	// Q,E ��ų �ִϸ��̼� -> ������ �ǿ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	UAnimMontage* QSkillMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	UAnimMontage* ESkillMontage;

	// ���� ��ų ���� ���⿡ ����� ����Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	UParticleSystemComponent* MagicParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main")
	bool bIsMagic;

	FName CombetCollisionName = TEXT("Root");

	//��ų ��ȣ
	int SkillNum;

	//�⺻������->�����Ϳ��� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	float UpDamage;

public:	
	// Sets default values for this actor's properties
	AWeapon(); // ������

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ������ ��
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//������ ���� ��
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//���� �Լ�
	UFUNCTION(BlueprintCallable)
	void Equip();

	// ���� �ߵ�
	void MagicActivate();
	// ���� ����
	void MagicDeactivate();

	UFUNCTION()
	void CombetOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void StartCollision();

	UFUNCTION(BlueprintCallable)
	void EndCollision();
};
