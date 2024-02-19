// Fill out your copyright notice in the Description page of Project Settings.


#include "Longsword_Weapon_Base.h"

ALongsword_Weapon_Base::ALongsword_Weapon_Base()
{
}

void ALongsword_Weapon_Base::BeginPlay()
{
	Super::BeginPlay();
}

void ALongsword_Weapon_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// ===============
// 충돌 이벤트 함수
// ===============
void ALongsword_Weapon_Base::OnHit(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, FVector _vNormalImpulse, const FHitResult& _Hit)
{
}

void ALongsword_Weapon_Base::OnOverlapBegin(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)
{
	Super::OnOverlapBegin(_PrimitiveCom, _OtherActor, _OtherPrimitiveCom, _Index, _bFromSweep, _HitResult);
}

void ALongsword_Weapon_Base::OnOverlapEnd(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index)
{
	Super::OnOverlapEnd(_PrimitiveCom, _OtherActor, _OtherPrimitiveCom, _Index);
}
