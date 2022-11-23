// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "MainCharacter.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//컴포넌트//
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh; //  RootComponent를 Mesh로 설정 -> AttachToComponent함수는 RootComponent 부착하는 함수이기 때문

	Collison = CreateDefaultSubobject<USphereComponent>(TEXT("Collsion"));
	Collison->SetupAttachment(RootComponent);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->SetupAttachment(RootComponent);
	Text->SetVisibility(false); // 게임 시작시 보이지 않음

	Collison->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 충돌 판정을 Overlap만 허용

	Collison->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapBegin); //스피어 콜리전 오버랩 이벤트 생성(Begin)
	Collison->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnOverlapEnd); //스피어 콜리전 오버랩 이벤트 생성(End)

	MagicParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MagicParticle")); // 파티클 시스템 컴포넌트 선언
	// 파티클 컴포넌트를 스켈레탈매쉬의 Root 본에 부착
	MagicParticle->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Mesh->GetSocketBoneName("Root"));
	MagicParticle->bAutoActivate = false; //자동 활성화 off -> 마법 스킬 사용시 이펙트 출력

	CombetCollison = CreateDefaultSubobject<UBoxComponent>(TEXT("CombetCollison"));
	CombetCollison->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, CombetCollisionName);
	CombetCollison->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombetOverlapBegin);

	//변수//
	Main = nullptr;
	EquipAble = false;
	bIsMagic = false;

	SocketName = "";
	ModeNum = 0;
	SkillNum = 0;
	Damage = 3.f;
	UpDamage = Damage * 0.5;

	EquipSound = nullptr;
	QSkillMontage = nullptr;
	ESkillMontage = nullptr;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	CombetCollison->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
			Main = Cast<AMainCharacter>(OtherActor); // 오버랩된 객체를 플레이로 형변환
			if (IsValid(Main))
			{
				Text->SetVisibility(true); // 택스트 보임
				EquipAble = true; // 장착 가능으로 변경
				EnableInput(UGameplayStatics::GetPlayerController(this, 0)); // 플레이어 입력 받음
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
		Main = nullptr; //메인 플레이어 nullptr로 변경
		Text->SetVisibility(false); // 택스트 숨김
		EquipAble = false; // 장착 불가로 변경
	}
}

//장착 함수
void AWeapon::Equip()
{
	if (EquipAble) // 장착 가능일때만 작동
	{
		Main->SetMode(ModeNum);
		if (Main->Mode < 4) // 검 + 대검 or 방패 + 대검 장착을 제한 -> 검+대검 or 방패 + 대검 은 Mode가 4가 되어 장착 불가
		{
			if (ActorHasTag("Weapon"))
			{
				Main->SettingWeapon(this);
			}
			
			UGameplayStatics::PlaySound2D(this, EquipSound);	//사운드 출력
			AttachToComponent(Main->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName); //지정된 소켓에 부착
			Text->SetVisibility(false); // 택스트 숨김
			EquipAble = false; // 장착 불가로 변경

			Main->QSkillMontage = QSkillMontage; // 스킬 애니메이션 동기화
			Main->ESkillMontage = ESkillMontage; // 스킬 애니메이션 동기화

			Collison->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 충돌 제거
		}
		else
		{
			Main->SetMode(-ModeNum); //중복 장착 시 Mode 값 초기화
		}
	}
}

//마법 발동시 무기에서 나오는 파티클 이펙트
void AWeapon::MagicActivate()
{
	if (!bIsMagic) // 마법이 발동 중이 아닐때
	{
		MagicParticle->ToggleActive(); // 파티클을 재생함
		bIsMagic = true; //마법 중으로 변경 
		Damage += UpDamage;
	}
	else
	{
		return;
	}

	FTimerHandle GravityTimerHandle;
	float GravityTime = 15.0f; // 버프의 지속 시간

	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			MagicDeactivate();
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);
	
}

// 버프 종료 함수 -> 타이머를 이용하여 버프 종료시 호출함
void AWeapon::MagicDeactivate()
{
	MagicParticle->ToggleActive();
	bIsMagic = false;
	Damage -= UpDamage;
}

//데미지 주기 평타 스킬 번호에 따라 다르게 계산
void AWeapon::CombetOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (IsValid(Enemy))
		{
			if (SkillNum == 1)
			{
				UGameplayStatics::ApplyDamage(Enemy, Damage * 2, NULL, this, NULL); 
			}
			else if (SkillNum == 2)
			{
				UGameplayStatics::ApplyDamage(Enemy, Damage * 0.8,  NULL, this, NULL);
			}
			else
			{
				UGameplayStatics::ApplyDamage(Enemy, Damage * 0.5, NULL, this, NULL);
			}
		}
		else { return; }
	}
	else { return; }
	UE_LOG(LogTemp, Warning, TEXT("OtherActor:: %s"), *OtherActor->GetName());
}

// 애니메이션BP에서 노티파이를 통해 호출 콜리전on/off
void AWeapon::StartCollision()
{
	CombetCollison->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::EndCollision()
{
	CombetCollison->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

