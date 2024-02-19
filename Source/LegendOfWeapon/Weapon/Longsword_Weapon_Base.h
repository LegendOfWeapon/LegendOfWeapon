// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon_Base.h"
#include "Longsword_Weapon_Base.generated.h"

/**
 * 
 */
UCLASS()
class LEGENDOFWEAPON_API ALongsword_Weapon_Base : public AWeapon_Base
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALongsword_Weapon_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// �浹 ���� Delegate
	virtual void OnHit(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom
		, FVector  _vNormalImpulse, const FHitResult& _Hit) override;


	virtual void OnOverlapBegin(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom
		, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)override;


	virtual void OnOverlapEnd(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom
		, int32 _Index)override;
};
