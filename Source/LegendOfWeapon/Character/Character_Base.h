// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/global.h"
#include "../Private/Interface_AnimInstances.h"
#include "../Private/Interface_PlayMontages.h"
#include "../Private/Interface_isAttacking.h"
#include "../Weapon/Weapon_Base.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

#include "../System/IADataAsset.h"
#include "InputMappingContext.h" 

#include "CoreMinimal.h"

#include "Character_Base.generated.h"



UCLASS()
class LEGENDOFWEAPON_API ACharacter_Base : public ACharacter, public IInterface_isAttacking
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UCameraComponent*				m_Cam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	USpringArmComponent*			m_Arm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeapon_Base>		m_Weapon;

	// reference to the own weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeapon_Base* EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool							IsLightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool							IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
	ECharacterState					CharacterStates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Enum")
	EAttackTypes					AttackType;

	void SpawnWeapon();


	bool bIsWPressed = false;
	bool bIsSPressed = false;
	bool bIsDPressed = false;
	bool bIsAPressed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bool")
	bool bIsAttacking = false;

	virtual void SendAttackNotification_Implementation(bool isAttacking) override;

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


#pragma region Movement
		FVector  Location = FVector();
		FRotator Rotation = FRotator();

		FVector  ForwardVector = FVector();
		FVector  RightVector = FVector();
		FVector  Velocity = FVector();
		FVector  Accel = FVector();

		float    GroundSpeed = 0.0f;
		float	 Speed = 0.0f;
		float	 Direction = 0.0f;

		FRotator MouseAim = FRotator();

		bool     IsFall = false;
		bool     IsMove = false;
#pragma endregion

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext>	InputMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UIADataAsset>			InputActionSetting;

	void DriveAnimVar();
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
	void MoveCanceled(const FInputActionInstance& _Instance);

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
	virtual void OnOverlapBegin(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom
		, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom
		, int32 _Index);

	public:
		// Declare RPC function perform in server
		UFUNCTION(Server, Reliable)
		void Server_PerformLightAttack();

		UFUNCTION(Server, Reliable)
		void Server_PerformLightAttackCancel();

		UFUNCTION(Server, Reliable)
		void Server_PerformMiddleAttack();

		UFUNCTION(Server, Reliable)
		void Server_PerformMiddleAttackCancel();

		UFUNCTION(Server, Reliable)
		void Server_PerformHeavyAttack();

		UFUNCTION(Server, Reliable)
		void Server_PerformHeavyAttackCancel();

		UFUNCTION(Server, Reliable)
		void Server_PerformBlock();

		UFUNCTION(Server, Reliable)
		void Server_PerformBlockComplete();

		UFUNCTION(NetMulticast, Reliable)
		void LightAttack();
		UFUNCTION(NetMulticast, Reliable)
		void LightAttackCancel();

		UFUNCTION(NetMulticast, Reliable)
		void MiddleAttack();
		UFUNCTION(NetMulticast, Reliable)
		void MiddleAttackCancel();

		UFUNCTION(NetMulticast, Reliable)
		void HeavyAttack();
		UFUNCTION(NetMulticast, Reliable)
		void HeavyAttackCancel();

		UFUNCTION(NetMulticast, Reliable)
		void BlockingOn();
		UFUNCTION(NetMulticast, Reliable)
		void BlockComplete();

		UFUNCTION(Server, Reliable)
		void ServerSetAttackTypes(EAttackTypes _AttackType);

		UFUNCTION(BlueprintCallable, Category = "HitDetection")
		void HitDetect();

		UFUNCTION(BlueprintCallable, Category = "HitDetection")
		void EndHitDetect();
};
