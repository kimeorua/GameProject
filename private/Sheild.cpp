// Fill out your copyright notice in the Description page of Project Settings.

#include "Sheild.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "Sound/SoundCue.h"
#include "particles/ParticleSystemComponent.h"

// Sets default values
ASheild::ASheild()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh; //  RootComponent를 Mesh로 설정 -> AttachToComponent함수는 RootComponent 부착하는 함수이기 때문

	Collison = CreateDefaultSubobject<USphereComponent>(TEXT("Collsion"));
	Collison->SetupAttachment(RootComponent);
	Collison->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 충돌 판정을 Overlap만 허용
	Collison->OnComponentBeginOverlap.AddDynamic(this, &ASheild::OnOverlapBegin); //스피어 콜리전 오버랩 이벤트 생성(Begin)
	Collison->OnComponentEndOverlap.AddDynamic(this, &ASheild::OnOverlapEnd); //스피어 콜리전 오버랩 이벤트 생성(End)

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->SetupAttachment(RootComponent);
	Text->SetVisibility(false); // 게임 시작시 보이지 않음

	BlockCollison = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockCollison"));
	BlockCollison->SetupAttachment(RootComponent);

	BlockParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BlockParticle"));
	ParringParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParringParticle"));
	

	Main = nullptr;
	EquipAble = false;

	SocketName = "";
	ModeNum = 0;

	EquipSound = nullptr;
	BlockSound = nullptr;
	ParryingSound = nullptr;
}

void ASheild::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void ASheild::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		Main = nullptr; //메인 플레이어 nullptr로 변경
		Text->SetVisibility(false); // 택스트 숨김
		EquipAble = false; // 장착 불가로 변경
	}
}

void ASheild::BlockCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (IsValid(Enemy))
		{
			bUseBlock = true;
		}
		else
		{
			return;
		}
	}
}

void ASheild::Equip()
{
	if (EquipAble) // 장착 가능일때만 작동
	{
		Main->SetMode(ModeNum);
		if (Main->Mode < 4) // 검 + 대검 or 방패 + 대검 장착을 제한 -> 검+대검 or 방패 + 대검 은 Mode가 4가 되어 장착 불가
		{
			if (ActorHasTag("Sheild"))
			{
				Main->SettingSheild(this);
			}
			UGameplayStatics::PlaySound2D(this, EquipSound);	//사운드 출력
			AttachToComponent(Main->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName); //지정된 소켓에 부착
			Text->SetVisibility(false); // 택스트 숨김
			EquipAble = false; // 장착 불가로 변경
			Collison->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 충돌 제거
		}
		else
		{
			Main->SetMode(-ModeNum); //중복 장착 시 Mode 값 초기화
		}
	}
}

void ASheild::EndCollision()
{
	BlockCollison->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASheild::PlayBloackSound()
{
	BlockParticle->ToggleActive();
	UGameplayStatics::PlaySound2D(this, BlockSound);

	FTimerHandle GravityTimerHandle;
	float GravityTime = 0.1f;
	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			BlockParticle->ToggleActive();
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);
}

void ASheild::PlayParringSound()
{
	ParringParticle->ToggleActive();
	UGameplayStatics::PlaySound2D(this, ParryingSound);

	FTimerHandle GravityTimerHandle;
	float GravityTime = 0.1f; 
	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			ParringParticle->ToggleActive();
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);
}

// Called when the game starts or when spawned
void ASheild::BeginPlay()
{
	Super::BeginPlay();

	BlockCollison->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BlockCollison->OnComponentBeginOverlap.AddDynamic(this, &ASheild::BlockCollisionOverlapBegin);

	BlockParticle->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Mesh->GetSocketBoneName("Root"));
	BlockParticle->bAutoActivate = false; //자동 활성화 off -> 마법 스킬 사용시 이펙트 출력
	ParringParticle->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Mesh->GetSocketBoneName("Root"));
	ParringParticle->bAutoActivate = false; //자동 활성화 off -> 마법 스킬 사용시 이펙트 출력

	bUseBlock = false;
}

// Called every frame
void ASheild::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

