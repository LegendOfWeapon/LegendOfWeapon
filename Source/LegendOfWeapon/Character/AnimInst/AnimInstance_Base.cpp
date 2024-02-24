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


	UpdateLocoMotionVariables(_fDeltaTime);
}

void UAnimInstance_Base::UpdateLocoMotionVariables(float _fDeltaTime)
{
	if(IsValid(Character))
	{
		DistplacementSinceLastUpdate = (WorldLocation - PreTickLastLocation).Size2D();

		PreTickLastLocation = WorldLocation;

	}
}


//void UAnimInstance_Base::SendCharacterState_Implementation(ECharacterState _State)
//{
//	CharacterStates = _State;
//	UE_LOG(LogTemp, Warning, TEXT("Current State is ??"));
//}
//
//void UAnimInstance_Base::SendAttackTypes_Implementation(EAttackTypes _Attack)
//{
//	AttackTypes = _Attack;
//}


