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
	bReplicates = true;
}

void ACharacter_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//���� ü�� ���ø�����Ʈ
	DOREPLIFETIME(ACharacter_Base, CurrentHealth);
	DOREPLIFETIME(ACharacter_Base, AttackType);
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

	SpawnWeapon();

	// �浹 �� ȣ���� �Լ� ���ε�
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACharacter_Base::OnHit);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_Base::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ACharacter_Base::OnOverlapEnd);
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
				InputCom->BindAction(pDA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Canceled, this, &ACharacter_Base::MoveCanceled);
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
					InputCom->BindAction(pDA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Canceled, this, &ACharacter_Base::BlockCompleted);
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

void ACharacter_Base::ServerSetAttackTypes_Implementation(EAttackTypes _AttackTypes)
{
	AttackType = _AttackTypes;
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
	AttackType = EAttackTypes::LightAttack;
	if (vInput.X > 0.f && vInput.Y == 0.f)
	{
		bIsWPressed = true;
		bIsSPressed = false;
		bIsAPressed = false;
		bIsDPressed = false;
		AttackType = EAttackTypes::W_LightAttack;
		ServerSetAttackTypes(AttackType);
	}
	else if (vInput.X < 0.f && vInput.Y == 0.f)
	{
		bIsWPressed = false;
		bIsSPressed = true;
		bIsAPressed = false;
		bIsDPressed = false;
		AttackType = EAttackTypes::S_LightAttack;
		ServerSetAttackTypes(AttackType);
	}
	else if (vInput.X == 0.f && vInput.Y == 0.f)
	{
		bIsWPressed = false;
		bIsSPressed = false;
		bIsAPressed = false;
		bIsDPressed = false;
		AttackType = EAttackTypes::LightAttack;
		ServerSetAttackTypes(AttackType);
	}
	else if (vInput.X == 0.f && vInput.Y > 0.f)
	{
		bIsWPressed = false;
		bIsSPressed = false;
		bIsAPressed = true;
		bIsDPressed = false;
		AttackType = EAttackTypes::A_LightAttack;
		ServerSetAttackTypes(AttackType);
	}
	else if (vInput.X == 0.f && vInput.Y < 0.f)
	{
		bIsWPressed = false;
		bIsSPressed = false;
		bIsAPressed = false;
		bIsDPressed = true;
		AttackType = EAttackTypes::D_LightAttack;
		ServerSetAttackTypes(AttackType);
	}
	else if (vInput.X > 0.f && vInput.Y > 0.f)
	{
		bIsWPressed = true;
		bIsSPressed = false;
		bIsAPressed = true;
		bIsDPressed = false;
		AttackType = EAttackTypes::LightAttack;
		ServerSetAttackTypes(AttackType);
	}
	else if (vInput.X > 0.f && vInput.Y < 0.f)
	{
		bIsWPressed = true;
		bIsSPressed = false;
		bIsAPressed = false;
		bIsDPressed = true;
		AttackType = EAttackTypes::LightAttack;
		ServerSetAttackTypes(AttackType);
	}
	else if (vInput.X < 0.f && vInput.Y > 0.f)
	{
		bIsWPressed = false;
		bIsSPressed = true;
		bIsAPressed = true;
		bIsDPressed = false;
		AttackType = EAttackTypes::LightAttack;
		ServerSetAttackTypes(AttackType);
	}
	else if (vInput.X < 0.f && vInput.Y < 0.f)
	{
		bIsWPressed = false;
		bIsSPressed = true;
		bIsAPressed = false;
		bIsDPressed = true;
		AttackType = EAttackTypes::LightAttack;
		ServerSetAttackTypes(AttackType);
	}


	if (vInput.X != 0.f && !bIsAttacking)
		GetCharacterMovement()->AddInputVector(GetActorForwardVector() * vInput.X);
	if (vInput.Y != 0.f && !bIsAttacking)
		GetCharacterMovement()->AddInputVector(GetActorRightVector() * vInput.Y);
}

void ACharacter_Base::MoveCanceled(const FInputActionInstance& _Instance)
{
	//UE_LOG(LogTemp, Warning, TEXT("Character MoveCanceled Function"));
	//GetCharacterMovement()->StopMovementImmediately();
	AttackType = EAttackTypes::LightAttack;
	ServerSetAttackTypes(AttackType);
}

void ACharacter_Base::LightAttack_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("LightAttack() function called"));
		//UE_LOG(LogTemp, Warning, TEXT("Anim Instance is not null!"));
		//Execute SendCharacterState
		if (AnimInstance->Implements<UInterface_AnimInstances>())
		{
			//UE_LOG(LogTemp, Warning, TEXT("interface implemented"));
			IInterface_AnimInstances::Execute_SendCharacterState(AnimInstance, ECharacterState::Attacking);
		}
		if (AnimInstance->Implements<UInterface_PlayMontages>())
		{
			IInterface_PlayMontages::Execute_SendAttackTypes(AnimInstance, AttackType);
		}
	}
}

void ACharacter_Base::MiddleAttack_Implementation() 
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

void ACharacter_Base::HeavyAttack_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Character HeavyAttack triggered"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		if (AnimInstance->Implements<UInterface_AnimInstances>())
		{
			IInterface_AnimInstances::Execute_SendCharacterState(AnimInstance, ECharacterState::Attacking);
		}
		if (AnimInstance->Implements<UInterface_PlayMontages>())
		{
			IInterface_PlayMontages::Execute_SendAttackTypes(AnimInstance, EAttackTypes::HeavyAttack);
		}
	}
}

void ACharacter_Base::BlockingOn_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Character Block triggered"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		if (AnimInstance->Implements<UInterface_AnimInstances>())
		{
			IInterface_AnimInstances::Execute_SendCharacterState(AnimInstance, ECharacterState::Blocking);
		}
	}
}

void ACharacter_Base::LightAttackCancel_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Character LightAttack Canceled"));
	//send attack type and character state
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	{
		if (AnimInstance->Implements<UInterface_AnimInstances>())
		{
			IInterface_AnimInstances::Execute_SendCharacterState(AnimInstance, ECharacterState::Idle);
		}
	}
}

void ACharacter_Base::MiddleAttackCancel_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Character MiddleAttack Canceled"));
	//send attack type and character state
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	{
		if (AnimInstance->Implements<UInterface_AnimInstances>())
		{
			IInterface_AnimInstances::Execute_SendCharacterState(AnimInstance, ECharacterState::Idle);
		}
	}
}

void ACharacter_Base::HeavyAttackCancel_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Character HeavyAttack Canceled"));
	//send attack type and character state
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	{
		if (AnimInstance->Implements<UInterface_AnimInstances>())
		{
			IInterface_AnimInstances::Execute_SendCharacterState(AnimInstance, ECharacterState::Idle);
		}
	}
}

void ACharacter_Base::BlockComplete_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Character Block Completed"));
	//send attack type and character state
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	{
		if (AnimInstance->Implements<UInterface_AnimInstances>())
		{
			IInterface_AnimInstances::Execute_SendCharacterState(AnimInstance, ECharacterState::Idle);
		}
	}
}

void ACharacter_Base::Server_PerformLightAttack_Implementation()
{
	LightAttack();
}

void ACharacter_Base::Server_PerformMiddleAttack_Implementation()
{
	MiddleAttack();
}

void ACharacter_Base::Server_PerformHeavyAttack_Implementation()
{
	HeavyAttack();
}

void ACharacter_Base::Server_PerformBlock_Implementation()
{
	BlockingOn();
}

void ACharacter_Base::Server_PerformLightAttackCancel_Implementation()
{
	LightAttackCancel();
}

void ACharacter_Base::Server_PerformMiddleAttackCancel_Implementation()
{
	MiddleAttackCancel();
}

void ACharacter_Base::Server_PerformHeavyAttackCancel_Implementation()
{
	HeavyAttackCancel();
}

void ACharacter_Base::Server_PerformBlockComplete_Implementation()
{
	BlockComplete();
}

//bool ACharacter_Base::Server_PerformLightAttack_Validate()
//{
//	// If you want to check if the player is allowed to perform the action
//	return true;
//}


void ACharacter_Base::LightAttackTriggered(const FInputActionInstance& _Instance)
{
	if (HasAuthority())
	{
			UE_LOG(LogTemp, Warning, TEXT("Server :::: LightAttack()"));
			LightAttack();
	}
	if (IsLocallyControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client:::: Server_PerformLightAttack()"));
		Server_PerformLightAttack();
	}
}


void ACharacter_Base::MiddleAttackTriggered(const FInputActionInstance& _Instance)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server :::: MiddleAttack()"));
		MiddleAttack();
	}
	if (IsLocallyControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client:::: Server_PerformMiddleAttack()"));
		Server_PerformMiddleAttack();
	}
}


void ACharacter_Base::HeavyAttackTriggered(const FInputActionInstance& _Instance)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server :::: HeavyAttack()"));
		HeavyAttack();
	}
	if (IsLocallyControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client:::: Server_PerformHeavyAttack()"));
		Server_PerformHeavyAttack();
	}
}

void ACharacter_Base::LightAttackCanceled(const FInputActionInstance& _Instance)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Character LightAttack Canceled"));
		LightAttackCancel();
	}
	else
	{
		Server_PerformLightAttackCancel();
	}
}

void ACharacter_Base::MiddleAttackCanceled(const FInputActionInstance& _Instance)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Character MiddleAttack Canceled"));
		MiddleAttackCancel();
	}
	else
	{
		Server_PerformMiddleAttackCancel();
	}
}

void ACharacter_Base::HeavyAttackCanceled(const FInputActionInstance& _Instance)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Character HeavyAttack Canceled"));
		HeavyAttackCancel();
	}
	else
	{
		Server_PerformHeavyAttackCancel();
	}
}


void ACharacter_Base::BlockTriggered(const FInputActionInstance& _Instance)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Block Triggered"));
		BlockingOn();
	}
	else
	{
		Server_PerformBlock();
	}
}

void ACharacter_Base::BlockCompleted(const FInputActionInstance& _Instance)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Block Completed"));
		BlockComplete();
	}
	else
	{
		Server_PerformBlockComplete();
	}
}


void ACharacter_Base::SendAttackNotification_Implementation(bool isAttacking)
{
	bIsAttacking = isAttacking;
}


void ACharacter_Base::ResetCombo()
{
	bCanCombo = false;
	// �޺� Ÿ�̸� ���
	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);
}

void ACharacter_Base::SpawnWeapon()
{
	if (m_Weapon) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// 무기 인스턴스 생성
		EquippedWeapon = GetWorld()->SpawnActor<AWeapon_Base>(m_Weapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (EquippedWeapon)
		{
			// 무기를 캐릭터에 장착하는 로직
			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
			EquippedWeapon->AttachToComponent(GetMesh(), AttachRules, TEXT("ik_hand_gunSocket"));
			//Hand_RSocket
			EquippedWeapon->m_pOwner = this;
		}
	}
}


// ===============
// �浹 �̺�Ʈ �Լ�
// ===============
void ACharacter_Base::OnHit(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, FVector _vNormalImpulse, const FHitResult& _Hit)
{
}

void ACharacter_Base::OnOverlapBegin(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)
{
}

void ACharacter_Base::OnOverlapEnd(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index)
{
}

//void ACharacter_Base::HitDetect1() 
//{
//	UE_LOG(LogTemp, Warning, TEXT("HitDetect() function called"));
//	// �浹 �̺�Ʈ �Լ� ���
//	TArray<AActor*> IgnoreActors;
//	IgnoreActors.Add(this);
//	FCollisionQueryParams CollisionParams;
//	CollisionParams.AddIgnoredActors(IgnoreActors);
//
//	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
//	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
//
//
//	//Eq
//
//	FVector vStart = GetMesh()->GetSocketLocation(TEXT("Hand_R"));
//	FVector vEnd = GetMesh()->GetSocketLocation(TEXT("Hand_R"));
//
//	FHitResult HitResult;
//	bool bResult = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), vStart, vEnd, 50.f, ObjectTypesArray, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Red, FLinearColor::Green, 5.f);
//
//	//Out hit
//
//	if (bResult)
//	{
//		//Break hit result
//
//
//
//		//Get hit actor
//		//Get hit actor's health component
//
//		//Apply damage
//
//		//Send hit notification
//
//		//UE_LOG(LogTemp, Warning, TEXT("HitDetect() function called"));
//
//	}	
//}

void ACharacter_Base::HitDetect() 
{

	// 무기의 BoxCollision 컴포넌트를 찾아 활성화 또는 비활성화
	if (EquippedWeapon)
	{
		UBoxComponent* BoxCollision = EquippedWeapon->FindComponentByClass<UBoxComponent>();
		if (BoxCollision)
		{
			BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			UE_LOG(LogTemp, Warning, TEXT("StartHitDetect?"));
		}
	}
}

void ACharacter_Base::EndHitDetect() 
{

	// 무기의 BoxCollision 컴포넌트를 찾아 활성화 또는 비활성화
	if (EquippedWeapon)
	{
		UBoxComponent* BoxCollision = EquippedWeapon->FindComponentByClass<UBoxComponent>();
		if (BoxCollision)
		{
			BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			UE_LOG(LogTemp, Warning, TEXT("EndHitDetect?"));
		}
	}
}

