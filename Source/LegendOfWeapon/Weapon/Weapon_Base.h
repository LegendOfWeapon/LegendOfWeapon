// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon_Base.generated.h"

UCLASS()
class LEGENDOFWEAPON_API AWeapon_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon_Base();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class ACharacter_Base* m_pOwner;  //  소유 플레이어

	// Damage amount
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float DamageAmount;

	// 무기의 메인 컴포넌트 (예: Mesh)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* WeaponMesh;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	// 충돌 관련 Delegate
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom
		, FVector  _vNormalImpulse, const FHitResult& _Hit);

	UFUNCTION(NetMulticast, Reliable)
	virtual void OnOverlapBegin(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom
		, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult);

	UFUNCTION(NetMulticast, Reliable)
	virtual void OnOverlapEnd(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom
		, int32 _Index);
};
