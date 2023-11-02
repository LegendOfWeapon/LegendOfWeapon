// Fill out your copyright notice in the Description page of Project Settings.
#include "Spear_AnimInst_Base.h"

#include "../SpearCharacter_Base.h"

void USpear_AnimInst_Base::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void USpear_AnimInst_Base::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void USpear_AnimInst_Base::NativeUpdateAnimation(float _fDeltaTime)
{
	Super::NativeUpdateAnimation(_fDeltaTime);

}


void USpear_AnimInst_Base::AnimNotify_Attack1End()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack1End Notify Called"));

	if (Character->ComboCount == 0)
		Character->IsLightAttack = false;

}

void USpear_AnimInst_Base::AnimNotify_Attack2End()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack2End Notify Called"));


	if (Character->ComboCount == 1)
	{
		Character->IsLightAttack = false;
		Character->ComboCount = 0;
	}
}

void USpear_AnimInst_Base::AnimNotify_AttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackEnd Notify Called"));
	Character->IsLightAttack = false;
	Character->ComboCount = 0;
}
