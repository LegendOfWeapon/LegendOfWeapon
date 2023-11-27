// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_Base.h"
#include "SpearCharacter_Base.generated.h"

/**
 * 
 */
UCLASS()
class LEGENDOFWEAPON_API ASpearCharacter_Base : public ACharacter_Base
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// Sets default values for this character's properties
	ASpearCharacter_Base();

private:
	virtual void LightAttackTriggered(const FInputActionInstance& _Instance) override;
	virtual void LightAttackCanceled(const FInputActionInstance& _Instance) override;
	virtual void MiddleAttackTriggered(const FInputActionInstance& _Instance) override;
	virtual void MiddleAttackCanceled(const FInputActionInstance& _Instance) override;
	virtual void HeavyAttackTriggered(const FInputActionInstance& _Instance) override;
	virtual void HeavyAttackCanceled(const FInputActionInstance& _Instance) override;
	virtual void BlockTriggered(const FInputActionInstance& _Instance) override;
	virtual void BlockCompleted(const FInputActionInstance& _Instance) override;

	// 충돌 관련 Delegate
	virtual void OnHit(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom
		, FVector  _vNormalImpulse, const FHitResult& _Hit) override;

	virtual void BeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom
		, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult) override;

	virtual void EndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom
		, int32 _Index) override;
};
