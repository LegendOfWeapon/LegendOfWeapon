// Fill out your copyright notice in the Description page of Project Settings.

#pragma once




#include "../Header/global.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IADataAsset.generated.h"


UCLASS()
class LEGENDOFWEAPON_API UIADataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "InputAction Setting"))
	TArray<FIAData>							IADataArr;
	
};
