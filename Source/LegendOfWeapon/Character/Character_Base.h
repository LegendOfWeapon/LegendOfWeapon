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
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UCameraComponent* m_Cam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	USpringArmComponent* m_Arm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool IsLightAttack;

	int32 ComboCount = 0;
	bool  bCanCombo  = false;


	UPROPERTY(EditAnywhere, Category = "Animation")
	TSoftObjectPtr<UAnimMontage>			DefaultMontage; // attack combo montage

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext>	InputMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UIADataAsset>			InputActionSetting;

public:
	TSoftObjectPtr<UAnimMontage> GetDefaultMontage() { return DefaultMontage; }

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
	FTimerHandle ComboTimerHandle;
	const float ComboWindowTime = 0.5f; // 콤보공격 입력 가능시간 
	void ComboAttack();
	void ResetCombo();
protected:
	// Triggered BindAction
	void Move(const FInputActionInstance& _Instance);

	UFUNCTION()
	virtual void LightAttack(const FInputActionInstance& _Instance);
         
	virtual void MiddleAttack(const FInputActionInstance& _Instance);		 
	virtual void HeavyAttack(const FInputActionInstance& _Instance);		  
};
