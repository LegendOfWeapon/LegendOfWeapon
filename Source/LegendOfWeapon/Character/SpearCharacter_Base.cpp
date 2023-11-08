// Fill out your copyright notice in the Description page of Project Settings.


#include "SpearCharacter_Base.h"

ASpearCharacter_Base::ASpearCharacter_Base()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// â ĳ������ ����Ʈ(�����޺�) ��Ÿ�ָ� ã�Ƽ� �����Ѵ�.
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

void ASpearCharacter_Base::LightAttack(const FInputActionInstance& _Instance)
{
	Super::LightAttack(_Instance);

	//// �ش� �׼ǽ� �����ؾ� �� �ڵ� �ۼ�
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
}

void ASpearCharacter_Base::BeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)
{
	Super::BeginOverlap( _PrimitiveCom,_OtherActor, _OtherPrimitiveCom, _Index, _bFromSweep,  _HitResult);
}

void ASpearCharacter_Base::EndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index)
{
	Super::EndOverlap( _PrimitiveCom,  _OtherActor,_OtherPrimitiveCom, _Index);
}
