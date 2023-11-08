// Fill out your copyright notice in the Description page of Project Settings.


#include "Spear_Weapon_Base.h"

ASpear_Weapon_Base::ASpear_Weapon_Base()
{
}

void ASpear_Weapon_Base::BeginPlay()
{
	Super::BeginPlay();

}

void ASpear_Weapon_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



// ===============
// 충돌 이벤트 함수
// ===============
void ASpear_Weapon_Base::OnHit(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, FVector _vNormalImpulse, const FHitResult& _Hit)
{
}

void ASpear_Weapon_Base::BeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)
{
	Super::BeginOverlap(_PrimitiveCom, _OtherActor, _OtherPrimitiveCom, _Index, _bFromSweep, _HitResult);
}

void ASpear_Weapon_Base::EndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index)
{
	Super::EndOverlap(_PrimitiveCom, _OtherActor, _OtherPrimitiveCom, _Index);
}

