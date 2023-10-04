// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "../System/IADataAsset.h"
#include "InputMappingContext.h" 

#include "CoreMinimal.h"
#include "Character_Base.generated.h"

UCLASS()
class LEGENDOFWEAPON_API ACharacter_Base : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext>	InputMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UIADataAsset>			InputActionSetting;


public:
	// Sets default values for this character's properties
	ACharacter_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Move(const FInputActionInstance& _Instance);
};
