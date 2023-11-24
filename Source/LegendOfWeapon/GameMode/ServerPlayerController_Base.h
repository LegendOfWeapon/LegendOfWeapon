// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ServerPlayerController_Base.generated.h"

/**
 * 
 */
UCLASS()
class LEGENDOFWEAPON_API AServerPlayerController_Base : public APlayerController
{
	GENERATED_BODY()
	
public:
	AServerPlayerController_Base();

protected:
	virtual void PostInitializeComponents() override;
	virtual void PostNetInit() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
};
