// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "Weapon_Base.generated.h"

/**
 * 
 */
UCLASS()
class LEGENDOFWEAPON_API AWeapon_Base : public ASkeletalMeshActor
{
	GENERATED_BODY()


public:
	virtual void BeginPlay() override;
	virtual void Tick(float _DT) override;
};
