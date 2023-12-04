// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_isAttacking.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_isAttacking : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IInterface_isAttacking
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION (BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
	void SendAttackNotification(bool isAttacking);
};
