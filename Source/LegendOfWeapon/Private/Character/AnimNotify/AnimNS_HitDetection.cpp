// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AnimNotify/AnimNS_HitDetection.h"

void UAnimNS_HitDetection::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference & EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp && MeshComp->GetOwner())
	{
		ACharacter_Base* Character = Cast<ACharacter_Base>(MeshComp->GetOwner());
		if (Character)
		{
			Character->HitDetect();
		}
	}
}

void UAnimNS_HitDetection::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference & EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UAnimNS_HitDetection::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference & EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp && MeshComp->GetOwner())
	{
		ACharacter_Base* Character = Cast<ACharacter_Base>(MeshComp->GetOwner());
		if (Character)
		{
			UE_LOG(LogTemp, Warning, TEXT("NotifyBegin: %s"), *MeshComp->GetOwner()->GetName());
			Character->EndHitDetect();
		}
	}
}