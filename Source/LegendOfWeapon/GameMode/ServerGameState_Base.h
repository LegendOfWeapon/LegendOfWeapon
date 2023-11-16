// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ServerGameState_Base.generated.h"

/**
 * 
 */
UCLASS()
class LEGENDOFWEAPON_API AServerGameState_Base : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void HandleBeginPlay() override;

protected:
	virtual void OnRep_ReplicatedHasBegunPlay() override;
};
