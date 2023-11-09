// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

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

protected:
	/** 플레이어의 최대 체력. 체력의 최댓값입니다. 이 값은 스폰 시 시작되는 캐릭터의 체력 값입니다.*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth;

	/** 플레이어의 현재 체력. 0이 되면 죽은 것으로 간주됩니다.*/
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
		float CurrentHealth;

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

	/** 프로퍼티 리플리케이션 */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** 현재 체력에 가해진 변경에 대한 RepNotify*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	/** 업데이트되는 체력에 반응. 서버에서는 수정 즉시 호출, 클라이언트에서는 RepNotify에 반응하여 호출*/
	void OnHealthUpdate();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** 현재 체력 세터. 값을 0과 MaxHealth 사이로 범위제한하고 OnHealthUpdate를 호출합니다. 서버에서만 호출되어야 합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetCurrentHealth(float healthValue);

	/** 대미지를 받는 이벤트. APawn에서 오버라이드됩니다.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public://Get(),Set() Collection
	/** 최대 체력 게터*/
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** 현재 체력 게터*/
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

private:	//Input
	FTimerHandle ComboTimerHandle;
	const float ComboWindowTime = 0.5f; // 콤보공격 입력 가능시간 
	void ComboAttack();
	void ResetCombo();
protected:
	// Triggered BindAction
	UFUNCTION()
	void Move(const FInputActionInstance& _Instance);
	UFUNCTION()
	virtual void LightAttack(const FInputActionInstance& _Instance);
	UFUNCTION()
	virtual void MiddleAttack(const FInputActionInstance& _Instance);
	UFUNCTION()
	virtual void HeavyAttack(const FInputActionInstance& _Instance);
};
