// Fill out your copyright notice in the Description page of Project Settings.


#include "./ServerGameState_Base.h"
#include "../LegendOfWeapon.h"

void AServerGameState_Base::HandleBeginPlay()
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::HandleBeginPlay();

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AServerGameState_Base::OnRep_ReplicatedHasBegunPlay()
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnRep_ReplicatedHasBegunPlay();

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}
