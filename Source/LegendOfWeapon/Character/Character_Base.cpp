// Fill out your copyright notice in the Description page of Project Settings.

#include "Character_Base.h"
#include "../Header/global.h"

#include "../GameMode/ServerPlayerController_Base.h"
#include "../ui/MainHUD_Base.h"
#include "../ui/PlayerInfo_Base.h"


#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../LegendOfWeapon.h"

// Sets default values
ACharacter_Base::ACharacter_Base()
	: m_Cam(nullptr)
	, m_Arm(nullptr)
	, IsLightAttack(false)
	, MaxHealth(100.f)
	, CurrentHealth(MaxHealth)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	m_Arm->SetupAttachment(GetCapsuleComponent());
	m_Cam->SetupAttachment(m_Arm);
}

void ACharacter_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//���� ü�� ���ø�����Ʈ
	DOREPLIFETIME(ACharacter_Base, CurrentHealth);
}

// Called when the game starts or when spawned
void ACharacter_Base::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* pController = Cast<APlayerController>(GetController());

	if (pController)
	{
		ULocalPlayer* pLocalPlayer = pController->GetLocalPlayer();

		if (pLocalPlayer && !InputMapping.IsNull())
		{
			// LocalPlayer�� EnhancedInput �ý����� ��, ����� IMC�� ����
			UEnhancedInputLocalPlayerSubsystem* pSubsystem = pLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			pSubsystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
		}
	}	

	// HUD ü�� ����
	AServerPlayerController_Base* pTempController = Cast<AServerPlayerController_Base>(GetController());
	if (pTempController && pTempController->IsLocalController())
	{
		UPlayerInfo_Base* PlayerInfoWidget = pTempController->GetMainHUD()->GetPlayerInfoWidget();
		PlayerInfoWidget->SetName(TEXT("ReSnow"));
		PlayerInfoWidget->SetHPBarRatio(1.f);
	}

	// �浹 �� ȣ���� �Լ� ���ε�
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACharacter_Base::OnHit);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_Base::BeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ACharacter_Base::EndOverlap);
}

void ACharacter_Base::PossessedBy(AController* NewController)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		AB_LOG(LogABNetwork, Log, TEXT("Owner : %s"), *OwnerActor->GetName());
	}
	else
	{
		AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("No Owner"));
	}

	Super::PossessedBy(NewController);

	OwnerActor = GetOwner();
	if (OwnerActor)
	{
		AB_LOG(LogABNetwork, Log, TEXT("Owner : %s"), *OwnerActor->GetName());
	}
	else
	{
		AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("No Owner"));
	}

	AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("End"), *GetName());
}

void ACharacter_Base::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void ACharacter_Base::OnHealthUpdate()
{
	//Ŭ���̾�Ʈ ���� �Լ� ���
	if (IsLocallyControlled())
	{
		// HUD ü�� ����
		AServerPlayerController_Base* pTempController = Cast<AServerPlayerController_Base>(GetController());
		if (pTempController)
		{
			UPlayerInfo_Base* PlayerInfoWidget = pTempController->GetMainHUD()->GetPlayerInfoWidget();
			
			if(CurrentHealth <=0) PlayerInfoWidget->SetHPBarRatio(0.f);
			else PlayerInfoWidget->SetHPBarRatio(CurrentHealth / MaxHealth);
		}


		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("You have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}


	}

	//���� ���� �Լ� ���
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	}
}

// Called every frame
void ACharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bIsWPressed = false;
	bIsSPressed = false;
	bIsAPressed = false;
	bIsDPressed = false;
}

// Called to bind functionality to input
void ACharacter_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// InputComponent -> EnhancedInputComponent �� �ٿ�ĳ����
	UEnhancedInputComponent* InputCom = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (nullptr == InputCom) return;


	if (!InputActionSetting.IsNull())
	{
		UIADataAsset* pDA = InputActionSetting.LoadSynchronous();

		for (int32 i = 0; i < pDA->IADataArr.Num(); ++i)
		{
			if (pDA->IADataArr[i].Action.IsNull())
				continue;

			switch (pDA->IADataArr[i].Type)
			{
			case EInputActionType::MOVE:
				InputCom->BindAction(pDA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ACharacter_Base::Move);
				break;

			case EInputActionType::LIGHT_ATTACK:
				InputCom->BindAction(pDA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ACharacter_Base::LightAttackTriggered);
				InputCom->BindAction(pDA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Canceled, this, &ACharacter_Base::LightAttackCanceled);
				break;

			case EInputActionType::MIDDLE_ATTACK:
				InputCom->BindAction(pDA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ACharacter_Base::MiddleAttackTriggered);
				InputCom->BindAction(pDA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Canceled, this, &ACharacter_Base::MiddleAttackCanceled);
				break;

			case EInputActionType::HEAVY_ATTACK:
				InputCom->BindAction(pDA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ACharacter_Base::HeavyAttackTriggered);
				InputCom->BindAction(pDA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Canceled, this, &ACharacter_Base::HeavyAttackCanceled);
				break;

			case EInputActionType::BLOCK:
					InputCom->BindAction(pDA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ACharacter_Base::BlockTriggered);
					InputCom->BindAction(pDA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Completed, this, &ACharacter_Base::BlockCompleted);
					break;

			}
		}
	}
}

void ACharacter_Base::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		UE_LOG(LogTemp, Warning, TEXT("Current HP is %d"), CurrentHealth);
		OnHealthUpdate();
	}
}

float ACharacter_Base::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageTaken, DamageEvent, EventInstigator, DamageCauser);
	
	float damageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(damageApplied);
	return damageApplied;
}


// ==================
// InputAction BindAction
// ==================
void ACharacter_Base::Move(const FInputActionInstance& _Instance)
{
	
	//UE_LOG(LogTemp, Warning, TEXT("Character Move Function"));

	FVector2D vInput = _Instance.GetValue().Get<FVector2D>();

	GetActorForwardVector()* vInput.X;
	GetActorForwardVector()* vInput.Y;

	if (vInput.X > 0.f && vInput.Y == 0.f)
	{
		bIsWPressed = true;
		bIsSPressed = false;
		bIsAPressed = false;
		bIsDPressed = false;
	}
	else if (vInput.X < 0.f && vInput.Y == 0.f)
	{
		bIsWPressed = false;
		bIsSPressed = true;
		bIsAPressed = false;
		bIsDPressed = false;
	}
	else if (vInput.X == 0.f && vInput.Y == 0.f)
	{
		bIsWPressed = false;
		bIsSPressed = false;
		bIsAPressed = false;
		bIsDPressed = false;
	}
	else if (vInput.X == 0.f && vInput.Y > 0.f)
	{
		bIsWPressed = false;
		bIsSPressed = false;
		bIsAPressed = true;
		bIsDPressed = false;
	}
	else if (vInput.X == 0.f && vInput.Y < 0.f)
	{
		bIsWPressed = false;
		bIsSPressed = false;
		bIsAPressed = false;
		bIsDPressed = true;
	}
	else if (vInput.X > 0.f && vInput.Y > 0.f)
	{
		bIsWPressed = true;
		bIsSPressed = false;
		bIsAPressed = true;
		bIsDPressed = false;
	}
	else if (vInput.X > 0.f && vInput.Y < 0.f)
	{
		bIsWPressed = true;
		bIsSPressed = false;
		bIsAPressed = false;
		bIsDPressed = true;
	}
	else if (vInput.X < 0.f && vInput.Y > 0.f)
	{
		bIsWPressed = false;
		bIsSPressed = true;
		bIsAPressed = true;
		bIsDPressed = false;
	}
	else if (vInput.X < 0.f && vInput.Y < 0.f)
	{
		bIsWPressed = false;
		bIsSPressed = true;
		bIsAPressed = false;
		bIsDPressed = true;
	}

	if (vInput.X != 0.f && !bIsAttacking)
		GetCharacterMovement()->AddInputVector(GetActorForwardVector() * vInput.X);
	if (vInput.Y != 0.f && !bIsAttacking)
		GetCharacterMovement()->AddInputVector(GetActorRightVector() * vInput.Y);
}

void ACharacter_Base::LightAttackTriggered(const FInputActionInstance& _Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Character LightAttack Triggered"));
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Anim Instance is not null!"));
		//Execute SendCharacterState
		if (AnimInstance->Implements<UInterface_AnimInstances>())
		{
			//UE_LOG(LogTemp, Warning, TEXT("interface implemented"));
			IInterface_AnimInstances::Execute_SendCharacterState(AnimInstance, ECharacterState::Attacking);
		}
		if (AnimInstance->Implements<UInterface_PlayMontages>())
		{
			if (bIsWPressed && !bIsAPressed && !bIsDPressed) // only W
			{
				IInterface_PlayMontages::Execute_SendAttackTypes(AnimInstance, EAttackTypes::W_LightAttack);
			}
			else if (bIsSPressed && !bIsAPressed && !bIsDPressed) // only S
			{
				IInterface_PlayMontages::Execute_SendAttackTypes(AnimInstance, EAttackTypes::S_LightAttack);
			}
			else if (bIsAPressed && !bIsWPressed && !bIsSPressed) // only A
			{
				IInterface_PlayMontages::Execute_SendAttackTypes(AnimInstance, EAttackTypes::A_LightAttack);
			}
			else if (bIsDPressed && !bIsWPressed && !bIsSPressed) // only D
			{
				IInterface_PlayMontages::Execute_SendAttackTypes(AnimInstance, EAttackTypes::D_LightAttack);
			}
			else // nothing or W + A or W + D or S + A or S + D
			{
				IInterface_PlayMontages::Execute_SendAttackTypes(AnimInstance, EAttackTypes::LightAttack);
			}
		}
	}
}

void ACharacter_Base::LightAttackCanceled(const FInputActionInstance& _Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Character LightAttack Canceled"));
	//send attack type and character state
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		if (AnimInstance->Implements<UInterface_AnimInstances>())
		{
			IInterface_AnimInstances::Execute_SendCharacterState(AnimInstance, ECharacterState::Idle);
		}
	}
}

void ACharacter_Base::MiddleAttackTriggered(const FInputActionInstance& _Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Character MiddleAttack triggered"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		if (AnimInstance->Implements<UInterface_AnimInstances>())
		{
			IInterface_AnimInstances::Execute_SendCharacterState(AnimInstance, ECharacterState::Attacking);
		}
		if (AnimInstance->Implements<UInterface_PlayMontages>())
		{
			IInterface_PlayMontages::Execute_SendAttackTypes(AnimInstance, EAttackTypes::MiddleAttack);
		}
	}
}

void ACharacter_Base::MiddleAttackCanceled(const FInputActionInstance& _Instance)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		if (AnimInstance->Implements<UInterface_AnimInstances>())
		{
			IInterface_AnimInstances::Execute_SendCharacterState(AnimInstance, ECharacterState::Idle);
		}
	}
}

void ACharacter_Base::HeavyAttackTriggered(const FInputActionInstance& _Instance)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		if (AnimInstance->Implements<UInterface_AnimInstances>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Character HeavyAttack triggered"));
			IInterface_AnimInstances::Execute_SendCharacterState(AnimInstance, ECharacterState::Attacking);
		}
		if (AnimInstance->Implements<UInterface_PlayMontages>())
		{
			IInterface_PlayMontages::Execute_SendAttackTypes(AnimInstance, EAttackTypes::HeavyAttack);
		}
	}
}

void ACharacter_Base::HeavyAttackCanceled(const FInputActionInstance& _Instance)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		if (AnimInstance->Implements<UInterface_AnimInstances>())
		{
			IInterface_AnimInstances::Execute_SendCharacterState(AnimInstance, ECharacterState::Idle);
		}
	}
}

void ACharacter_Base::BlockTriggered(const FInputActionInstance& _Instance)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr && !bIsAttacking)
	{
		if (AnimInstance->Implements<UInterface_AnimInstances>())
		{
			IInterface_AnimInstances::Execute_SendCharacterState(AnimInstance, ECharacterState::Blocking);
		}
	}
}

void ACharacter_Base::BlockCompleted(const FInputActionInstance& _Instance)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		if (AnimInstance->Implements<UInterface_AnimInstances>())
		{
			IInterface_AnimInstances::Execute_SendCharacterState(AnimInstance, ECharacterState::Idle);
		}
	}
}

void ACharacter_Base::SendAttackNotification_Implementation(bool isAttacking)
{
	bIsAttacking = isAttacking;
}

// ===============
// �浹 �̺�Ʈ �Լ�
// ===============
void ACharacter_Base::OnHit(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, FVector _vNormalImpulse, const FHitResult& _Hit)
{
}

void ACharacter_Base::BeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)
{
}

void ACharacter_Base::EndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index)
{
}

