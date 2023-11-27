// Fill out your copyright notice in the Description page of Project Settings.

#include "SpearCharacter_Base.h"

#include "../Weapon/Weapon_Base.h"
#include "Character_Base.h"

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

	// 무기 창(BPC_Spear)을 찾아서 참조한다. 
	ConstructorHelpers::FClassFinder<AWeapon_Base> weapon(TEXT("/Script/Engine.Blueprint'/Game/BluePrints/Weapon/BPC_Spear.BPC_Spear_C'"));
	if (weapon.Succeeded())
	{
		m_Weapon = weapon.Class;
	}

}

void ASpearCharacter_Base::BeginPlay()
{
	Super::BeginPlay();

	// 무기 창 생성
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	param.OverrideLevel = GetLevel();
	param.bDeferConstruction = false;
	param.Owner = this;

	FTransform WeaponSocketTransform =  GetMesh()->GetSocketTransform(TEXT("ik_hand_gunSocket"), RTS_World);
	AWeapon_Base* pWeapon = GetWorld()->SpawnActor<AWeapon_Base>(m_Weapon, WeaponSocketTransform, param);

	// 플레이어에게 창 장착
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
	pWeapon->AttachToComponent(GetMesh(), AttachRules, TEXT("ik_hand_gunSocket"));
	pWeapon->m_pOwner = this;
}

void ASpearCharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpearCharacter_Base::LightAttack(const FInputActionInstance& _Instance)
{
	//Super::LightAttack(_Instance);

	//// 해당 액션시 수행해야 할 코드 작성
	//UE_LOG(LogTemp, Warning, TEXT("Spear Character LightAttack"));
}

void ASpearCharacter_Base::MiddleAttack(const FInputActionInstance& _Instance)
{
}

void ASpearCharacter_Base::HeavyAttack(const FInputActionInstance& _Instance)
{
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
