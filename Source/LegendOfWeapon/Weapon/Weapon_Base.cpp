// Fill out your copyright notice in the Description page of Project Settings.
#include "Weapon_Base.h"


#include "../Character/Character_Base.h"

// Sets default values
AWeapon_Base::AWeapon_Base()
	:m_pOwner(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeapon_Base::BeginPlay()
{
	Super::BeginPlay();

	// 충돌 시 호출할 함수 바인딩
	UBoxComponent* BoxCollision = FindComponentByClass<UBoxComponent>();
	if (BoxCollision)
	{
		BoxCollision->OnComponentHit.AddDynamic(this, &AWeapon_Base::OnHit);
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon_Base::BeginOverlap);
		BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon_Base::EndOverlap);
	}

}

// Called every frame
void AWeapon_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon_Base::OnHit(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, FVector _vNormalImpulse, const FHitResult& _Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Weapon_Base OnHit"));
}

void AWeapon_Base::BeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)
{	
	ACharacter_Base* pPlayer = Cast<ACharacter_Base>(_OtherActor);

	// TODO 플레이어가 공격 뭐냐에 따라서 데미지를 주어야 한다. 
	if (IsValid(pPlayer) && m_pOwner!=pPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon_Base BeginOverlap"));
	}
}

void AWeapon_Base::EndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index)
{
}

