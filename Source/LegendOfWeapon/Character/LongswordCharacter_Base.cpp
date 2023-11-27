// Fill out your copyright notice in the Description page of Project Settings.


#include "LongswordCharacter_Base.h"

ALongswordCharacter_Base::ALongswordCharacter_Base()
{	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ALongswordCharacter_Base::BeginPlay()
{
	Super::BeginPlay();
}

void ALongswordCharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALongswordCharacter_Base::LightAttackTriggered(const FInputActionInstance& _Instance)
{	
	// 해당 액션시 수행해야 할 코드 작성
	UE_LOG(LogTemp, Warning, TEXT("Longsword Character LightAttack Triggered"));
}

void ALongswordCharacter_Base::LightAttackCanceled(const FInputActionInstance& _Instance)
{
	// 해당 액션시 수행해야 할 코드 작성
	UE_LOG(LogTemp, Warning, TEXT("Longsword Character LightAttack Canceled"));
}

void ALongswordCharacter_Base::MiddleAttackTriggered(const FInputActionInstance& _Instance)
{
}

void ALongswordCharacter_Base::MiddleAttackCanceled(const FInputActionInstance& _Instance)
{
}

void ALongswordCharacter_Base::HeavyAttackTriggered(const FInputActionInstance& _Instance)
{
}

void ALongswordCharacter_Base::HeavyAttackCanceled(const FInputActionInstance& _Instance)
{
}



void ALongswordCharacter_Base::OnHit(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, FVector _vNormalImpulse, const FHitResult& _Hit)
{
	Super::OnHit(_PrimitiveCom, _OtherActor, _OtherPrimitiveCom, _vNormalImpulse, _Hit);
}

void ALongswordCharacter_Base::BeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)
{
	Super::BeginOverlap(_PrimitiveCom, _OtherActor, _OtherPrimitiveCom, _Index, _bFromSweep, _HitResult);
}

void ALongswordCharacter_Base::EndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index)
{
	Super::EndOverlap(_PrimitiveCom, _OtherActor, _OtherPrimitiveCom, _Index);
}
