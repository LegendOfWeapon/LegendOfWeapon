// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance_Base.h"
#include "Spear_AnimInst_Base.generated.h"

/**
 * 
 */
UCLASS()
class LEGENDOFWEAPON_API USpear_AnimInst_Base : public UAnimInstance_Base
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _fDeltaTime) override;


public:
	// AniNotify_ + ��Ƽ���� �̸� ���� �Լ����� ������ ��, UFUNCTION() ��ũ�� �ݵ�� ���� ��
	UFUNCTION()
	void AnimNotify_Attack1End();

	UFUNCTION()
	void AnimNotify_Attack2End();

	UFUNCTION()
	void AnimNotify_AttackEnd();
};
