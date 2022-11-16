// Fill out your copyright notice in the Description page of Project Settings.

#include "Sheild.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"

// Sets default values
ASheild::ASheild()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh; //  RootComponent�� Mesh�� ���� -> AttachToComponent�Լ��� RootComponent �����ϴ� �Լ��̱� ����

	Collison = CreateDefaultSubobject<USphereComponent>(TEXT("Collsion"));
	Collison->SetupAttachment(RootComponent);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->SetupAttachment(RootComponent);
	Text->SetVisibility(false); // ���� ���۽� ������ ����

	Collison->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // �浹 ������ Overlap�� ���

	Collison->OnComponentBeginOverlap.AddDynamic(this, &ASheild::OnOverlapBegin); //���Ǿ� �ݸ��� ������ �̺�Ʈ ����(Begin)
	Collison->OnComponentEndOverlap.AddDynamic(this, &ASheild::OnOverlapEnd); //���Ǿ� �ݸ��� ������ �̺�Ʈ ����(End)

	Main = nullptr;
	EquipAble = false;

	SocketName = "";
	ModeNum = 0;

	EquipSound = nullptr;
}

void ASheild::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->IsA(AMainCharacter::StaticClass()))
		{
			Main = Cast<AMainCharacter>(OtherActor); // �������� ��ü�� �÷��̷� ����ȯ
			if (IsValid(Main))
			{
				Text->SetVisibility(true); // �ý�Ʈ ����
				EquipAble = true; // ���� �������� ����
				EnableInput(UGameplayStatics::GetPlayerController(this, 0)); // �÷��̾� �Է� ����
			}
		}
		else
		{
			return;
		}
	}
}

void ASheild::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		Main = nullptr; //���� �÷��̾� nullptr�� ����
		Text->SetVisibility(false); // �ý�Ʈ ����
		EquipAble = false; // ���� �Ұ��� ����
	}
}

void ASheild::Equip()
{
	if (EquipAble) // ���� �����϶��� �۵�
	{
		Main->SetMode(ModeNum);
		if (Main->Mode < 4) // �� + ��� or ���� + ��� ������ ���� -> ��+��� or ���� + ��� �� Mode�� 4�� �Ǿ� ���� �Ұ�
		{
			UGameplayStatics::PlaySound2D(this, EquipSound);	//���� ���
			AttachToComponent(Main->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName); //������ ���Ͽ� ����
			Text->SetVisibility(false); // �ý�Ʈ ����
			EquipAble = false; // ���� �Ұ��� ����
			Collison->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// �浹 ����
		}
		else
		{
			Main->SetMode(-ModeNum); //�ߺ� ���� �� Mode �� �ʱ�ȭ
		}
	}
}

// Called when the game starts or when spawned
void ASheild::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASheild::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

