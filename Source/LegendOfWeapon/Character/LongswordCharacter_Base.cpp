// Fill out your copyright notice in the Description page of Project Settings.


#include "LongswordCharacter_Base.h"
#include "../Weapon/Weapon_Base.h"

ALongswordCharacter_Base::ALongswordCharacter_Base()
{	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ALongswordCharacter_Base::BeginPlay()
{
	Super::BeginPlay();

	// 무기 롱소드 생성
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	param.OverrideLevel = GetLevel();
	param.bDeferConstruction = false;
	param.Owner = this;

	FTransform WeaponSocketTransform = GetMesh()->GetSocketTransform(TEXT("Hand_RSocket"), RTS_World);
	AWeapon_Base* pWeapon = GetWorld()->SpawnActor<AWeapon_Base>(m_Weapon, WeaponSocketTransform, param);

	if (pWeapon)
	{
		// 플레이어에게 롱소드 장착
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
		pWeapon->AttachToComponent(GetMesh(), AttachRules, TEXT("Hand_RSocket"));
		pWeapon->m_pOwner = this;
	}
}

void ALongswordCharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALongswordCharacter_Base::LightAttackTriggered(const FInputActionInstance& _Instance)
{	
	Super::LightAttackTriggered(_Instance);
}

void ALongswordCharacter_Base::LightAttackCanceled(const FInputActionInstance& _Instance)
{
	Super::LightAttackCanceled(_Instance);

}

void ALongswordCharacter_Base::MiddleAttackTriggered(const FInputActionInstance& _Instance)
{
	Super::MiddleAttackTriggered(_Instance);
}

void ALongswordCharacter_Base::MiddleAttackCanceled(const FInputActionInstance& _Instance)
{
	Super::MiddleAttackCanceled(_Instance);
}

void ALongswordCharacter_Base::HeavyAttackTriggered(const FInputActionInstance& _Instance)
{
	Super::HeavyAttackTriggered(_Instance);
}

void ALongswordCharacter_Base::HeavyAttackCanceled(const FInputActionInstance& _Instance)
{
	Super::HeavyAttackCanceled(_Instance);
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
