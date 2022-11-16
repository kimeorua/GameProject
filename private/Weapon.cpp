// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "MainCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//������Ʈ//
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

	Collison->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapBegin); //���Ǿ� �ݸ��� ������ �̺�Ʈ ����(Begin)
	Collison->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnOverlapEnd); //���Ǿ� �ݸ��� ������ �̺�Ʈ ����(End)

	MagicParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MagicParticle")); // ��ƼŬ �ý��� ������Ʈ ����
	// ��ƼŬ ������Ʈ�� ���̷�Ż�Ž��� Root ���� ����
	MagicParticle->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Mesh->GetSocketBoneName("Root"));
	MagicParticle->bAutoActivate = false; //�ڵ� Ȱ��ȭ off -> ���� ��ų ���� ����Ʈ ���

	//����//
	Main = nullptr;
	EquipAble = false;
	bIsMagic = false;

	SocketName = "";
	ModeNum = 0;

	EquipSound = nullptr;
	QSkillMontage = nullptr;
	ESkillMontage = nullptr;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
// OverlapEnd
void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		Main = nullptr; //���� �÷��̾� nullptr�� ����
		Text->SetVisibility(false); // �ý�Ʈ ����
		EquipAble = false; // ���� �Ұ��� ����
	}
}

//���� �Լ�
void AWeapon::Equip()
{
	if (EquipAble) // ���� �����϶��� �۵�
	{
		Main->SetMode(ModeNum);
		if (Main->Mode < 4) // �� + ��� or ���� + ��� ������ ���� -> ��+��� or ���� + ��� �� Mode�� 4�� �Ǿ� ���� �Ұ�
		{
			if (ActorHasTag("Weapon"))
			{
				Main->SettingWeapon(this);
			}
			
			UGameplayStatics::PlaySound2D(this, EquipSound);	//���� ���
			AttachToComponent(Main->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName); //������ ���Ͽ� ����
			Text->SetVisibility(false); // �ý�Ʈ ����
			EquipAble = false; // ���� �Ұ��� ����

			Main->QSkillMontage = QSkillMontage; // ��ų �ִϸ��̼� ����ȭ
			Main->ESkillMontage = ESkillMontage; // ��ų �ִϸ��̼� ����ȭ

			Collison->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// �浹 ����
		}
		else
		{
			Main->SetMode(-ModeNum); //�ߺ� ���� �� Mode �� �ʱ�ȭ
		}
	}
}

//���� �ߵ��� ���⿡�� ������ ��ƼŬ ����Ʈ
void AWeapon::MagicActivate()
{
	if (!bIsMagic) // ������ �ߵ� ���� �ƴҶ�
	{
		MagicParticle->ToggleActive(); // ��ƼŬ�� �����
		bIsMagic = true; //���� ������ ���� 
	}
	else
	{
		return;
	}

	FTimerHandle GravityTimerHandle;
	float GravityTime = 5.0f; // ������ ���� �ð�

	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			MagicDeactivate();
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);
	
}

// ���� ���� �Լ� -> Ÿ�̸Ӹ� �̿��Ͽ� ���� ����� ȣ����
void AWeapon::MagicDeactivate()
{
	MagicParticle->ToggleActive();
	bIsMagic = false;
}

