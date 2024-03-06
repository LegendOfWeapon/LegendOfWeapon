// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../Character_Base.h"
#include "../Header/global.h"
#include "../Private/Interface_AnimInstances.h"
#include "../Private/Interface_PlayMontages.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Base.generated.h"


/**
 * 
 */
UCLASS()
class LEGENDOFWEAPON_API UAnimInstance_Base : public UAnimInstance, public IInterface_AnimInstances, public IInterface_PlayMontages
{
	GENERATED_BODY()
public:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Reference")
	ACharacter_Base* Character;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Reference")
	UCharacterMovementComponent* Movement;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
	ECharacterState CharacterStates;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
	EAttackTypes AttackTypes;

	//Montage Name define
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FName LightComboAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FName DefaultLightComboAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FName MiddleComboAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FName DefaultMiddleComboAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FName HeavyComboAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FName DefaultHeavyComboAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bool")
	bool IsAttack;


	// LocoMotion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocoMotion")
	float DistplacementSinceLastUpdate = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocoMotion")
	FVector PreTickLastLocation = FVector(0.0f, 0.0f, 0.0f);


#pragma region Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector  Location = FVector();
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FRotator Rotation = FRotator();

 	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector  ForwardVector = FVector();
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector  RightVector = FVector();
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector  Velocity = FVector();
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector  Accel = FVector();

 	UPROPERTY(EditAnywhere, BlueprintReadWrite) float	 GroundSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float	 Speed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float	 Direction = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FRotator MouseAim = FRotator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool     IsFall = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool     IsMove = false;
#pragma endregion

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _fDeltaTime) override;

protected:
	void UpdateLocoMotionVariables(float _fDeltaTime);
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Send Character State")
	//void SendCharacterState(ECharacterState _CharacterState);
	//virtual void SendCharacterState_Implementation(ECharacterState _CharacterState) override;

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Play Montages")
	//void SendAttackTypes(EAttackTypes _AttackType);
	//virtual void SendAttackTypes_Implementation(EAttackTypes _AttackType) override;


	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Play Montages")
	//void SendCharacterState_Implementation(ECharacterState _State) override;

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Play Montages")
	//void SendAttackTypes_Implementation(EAttackTypes _Attack) override;
};