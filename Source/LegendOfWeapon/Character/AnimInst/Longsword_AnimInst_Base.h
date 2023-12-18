// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance_Base.h"
#include "Longsword_AnimInst_Base.generated.h"

/**
 * 
 */
UCLASS()
class LEGENDOFWEAPON_API ULongsword_AnimInst_Base : public UAnimInstance_Base
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _fDeltaTime) override;

};
