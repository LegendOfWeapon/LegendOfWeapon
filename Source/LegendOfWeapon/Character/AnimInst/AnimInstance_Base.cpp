// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Base.h"

void UAnimInstance_Base::NativeInitializeAnimation()
{
}

void UAnimInstance_Base::NativeBeginPlay()
{
	Character = Cast<ACharacter_Base>(GetOwningActor());

	if (IsValid(Character))
	{
		Movement = Character->GetCharacterMovement();
	}
}

void UAnimInstance_Base::NativeUpdateAnimation(float _fDeltaTime)
{
	if (!IsValid(Character) || !IsValid(Movement))
		return;
}
