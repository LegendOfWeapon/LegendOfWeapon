// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../Character_Base.h"

#include "Animation/AnimInstance.h"
#include "AnimInstance_Base.generated.h"

/**
 * 
 */
UCLASS()
class LEGENDOFWEAPON_API UAnimInstance_Base : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Reference")
	ACharacter_Base* Character;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Reference")
	UCharacterMovementComponent* Movement;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _fDeltaTime) override;
};