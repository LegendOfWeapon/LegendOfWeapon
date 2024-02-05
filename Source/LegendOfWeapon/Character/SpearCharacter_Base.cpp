// Fill out your copyright notice in the Description page of Project Settings.

#include "SpearCharacter_Base.h"
#include "../Weapon/Weapon_Base.h"

ASpearCharacter_Base::ASpearCharacter_Base()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 창 캐릭터의 디폴트(공격콤보) 몽타주를 찾아서 참조한다.
	ConstructorHelpers::FObjectFinder<UAnimMontage> montage(TEXT("/Script/Engine.AnimMontage'/Game/BluePrints/Player/Spear/Animation/Montages/Spear_Default_Montage.Spear_Default_Montage'"));
	if (montage.Succeeded())
	{
		DefaultMontage = montage.Object;
	}
}

void ASpearCharacter_Base::BeginPlay()
{
	Super::BeginPlay();
}



void ASpearCharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpearCharacter_Base::LightAttackTriggered(const FInputActionInstance& _Instance)
{
	Super::LightAttackTriggered(_Instance);

	//// 해당 액션시 수행해야 할 코드 작성
	UE_LOG(LogTemp, Warning, TEXT("Spear Character LightAttack Triggered inheritied"));
}

void ASpearCharacter_Base::LightAttackCanceled(const FInputActionInstance& _Instance)
{
	Super::LightAttackCanceled(_Instance);

	//// 해당 액션시 수행해야 할 코드 작성
	UE_LOG(LogTemp, Warning, TEXT("Spear Character LightAttack Canceled"));
}

void ASpearCharacter_Base::MiddleAttackTriggered(const FInputActionInstance& _Instance)
{
	Super::MiddleAttackTriggered(_Instance);
}

void ASpearCharacter_Base::MiddleAttackCanceled(const FInputActionInstance& _Instance)
{
	Super::MiddleAttackCanceled(_Instance);
}

void ASpearCharacter_Base::HeavyAttackTriggered(const FInputActionInstance& _Instance)
{
	Super::HeavyAttackTriggered(_Instance);
}

void ASpearCharacter_Base::HeavyAttackCanceled(const FInputActionInstance& _Instance)
{
	Super::HeavyAttackCanceled(_Instance);
}

void ASpearCharacter_Base::BlockTriggered(const FInputActionInstance& _Instance)
{
	Super::BlockTriggered(_Instance);
}

void ASpearCharacter_Base::BlockCompleted(const FInputActionInstance& _Instance)
{
	Super::BlockCompleted(_Instance);
}

void ASpearCharacter_Base::OnHit(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, FVector _vNormalImpulse, const FHitResult& _Hit)
{
	Super::OnHit(_PrimitiveCom, _OtherActor, _OtherPrimitiveCom, _vNormalImpulse, _Hit);
}

void ASpearCharacter_Base::BeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)
{
	Super::BeginOverlap( _PrimitiveCom,_OtherActor, _OtherPrimitiveCom, _Index, _bFromSweep,  _HitResult);
}

void ASpearCharacter_Base::EndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index)
{
	Super::EndOverlap( _PrimitiveCom,  _OtherActor,_OtherPrimitiveCom, _Index);
}
