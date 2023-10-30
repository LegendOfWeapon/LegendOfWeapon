// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_Base.h"
#include "LongswordCharacter_Base.generated.h"

/**
 * 
 */
UCLASS()
class LEGENDOFWEAPON_API ALongswordCharacter_Base : public ACharacter_Base
{
	GENERATED_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// Sets default values for this character's properties
	ALongswordCharacter_Base();

};
