// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/global.h"
#include "../Private/Interface_AnimInstances.h"
#include "../Private/Interface_PlayMontages.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

#include "../System/IADataAsset.h"
#include "InputMappingContext.h" 

#include "CoreMinimal.h"
#include "Character_Base.generated.h"

class AWeapon_Base;

UCLASS()
class LEGENDOFWEAPON_API ACharacter_Base : public ACharacter
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UCameraComponent*				m_Cam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	USpringArmComponent*			m_Arm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TSubclassOf<AWeapon_Base>		m_Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool							IsLightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool							IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
	ECharacterState					CharacterStates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
	EAttackTypes					AttackTypes;


	bool bIsWPressed = false;
	bool bIsSPressed = false;
	bool bIsDPressed = false;
	bool bIsAPressed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bool")
	bool bIsAttacking = false;

	int32 ComboCount = 0;
	bool  bCanCombo  = false;


	UPROPERTY(EditAnywhere, Category = "Animation")
	TSoftObjectPtr<UAnimMontage>	DefaultMontage; // attack combo montage

protected:
	/** �÷��̾��� �ִ� ü��. ü���� �ִ��Դϴ�. �� ���� ���� �� ���۵Ǵ� ĳ������ ü�� ���Դϴ�.*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth;

	/** �÷��̾��� ���� ü��. 0�� �Ǹ� ���� ������ ���ֵ˴ϴ�.*/
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

	/** ������Ƽ ���ø����̼� */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	/** ���� ü�¿� ������ ���濡 ���� RepNotify*/
	UFUNCTION()
	void OnRep_CurrentHealth();	

	/** ������Ʈ�Ǵ� ü�¿� ����. ���������� ���� ��� ȣ��, Ŭ���̾�Ʈ������ RepNotify�� �����Ͽ� ȣ��*/
	void OnHealthUpdate();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** ���� ü�� ����. ���� 0�� MaxHealth ���̷� ���������ϰ� OnHealthUpdate�� ȣ���մϴ�. ���������� ȣ��Ǿ�� �մϴ�.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetCurrentHealth(float healthValue);

	/** ������� �޴� �̺�Ʈ. APawn���� �������̵�˴ϴ�.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public://Get(),Set() Collection
	/** �ִ� ü�� ����*/
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** ���� ü�� ����*/
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

private:	
	FTimerHandle ComboTimerHandle;
	const float ComboWindowTime = 0.5f; // �޺����� �Է� ���ɽð� 
	void ComboAttack();
	void ResetCombo();
protected:
	// Triggered BindAction
	void Move(const FInputActionInstance& _Instance);

	UFUNCTION()
	virtual void LightAttackTriggered(const FInputActionInstance& _Instance);
	UFUNCTION()
	virtual void LightAttackCanceled(const FInputActionInstance& _Instance);
	UFUNCTION()
	virtual void MiddleAttackTriggered(const FInputActionInstance& _Instance);
	UFUNCTION()
	virtual void MiddleAttackCanceled(const FInputActionInstance& _Instance);
	UFUNCTION()
	virtual void HeavyAttackTriggered(const FInputActionInstance& _Instance);
	UFUNCTION()
	virtual void HeavyAttackCanceled(const FInputActionInstance& _Instance);
	UFUNCTION()
	virtual void BlockTriggered(const FInputActionInstance& _Instance);
	UFUNCTION()
	virtual void BlockCompleted(const FInputActionInstance& _Instance);

	// �浹 ���� Delegate
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom
		, FVector  _vNormalImpulse, const FHitResult& _Hit);

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom
		, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom
		, int32 _Index);
};
