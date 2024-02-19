// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Character/Character_Base.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNS_HitDetection.generated.h"


/**
 * 
 */
UCLASS()
class LEGENDOFWEAPON_API UAnimNS_HitDetection : public UAnimNotifyState
{
	GENERATED_BODY()
	

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference & EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference & EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference & EventReference) override;

};
// Path: Source/LegendOfWeapon/Private/Character/AnimNotify/AnimNS_HitDetection.cpp