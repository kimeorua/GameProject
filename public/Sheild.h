// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sheild.generated.h"

UCLASS()
class CPP_MAGNUSBELLATROR_API ASheild : public AActor
{
	GENERATED_BODY()
	
private:
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

public:	
	// Sets default values for this actor's properties
	ASheild();

	// ������ ��
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//������ ���� ��
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void Equip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
