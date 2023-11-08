// Fill out your copyright notice in the Description page of Project Settings.

#include "Character_Base.h"
#include "../Header/global.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

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

	//현재 체력 리플리케이트
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
			// LocalPlayer의 EnhancedInput 시스템을 얻어서, 사용할 IMC를 세팅
			UEnhancedInputLocalPlayerSubsystem* pSubsystem = pLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			pSubsystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
		}
	}
}

void ACharacter_Base::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void ACharacter_Base::OnHealthUpdate()
{
	//클라이언트 전용 함수 기능
	if (IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("You have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	//서버 전용 함수 기능
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

}

// Called to bind functionality to input
void ACharacter_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// InputComponent -> EnhancedInputComponent 로 다운캐스팅
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
				InputCom->BindAction(pDA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ACharacter_Base::LightAttack);
				break;

			case EInputActionType::MIDDLE_ATTACK:
				InputCom->BindAction(pDA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ACharacter_Base::MiddleAttack);
				break;

			case EInputActionType::HEAVY_ATTACK:
				InputCom->BindAction(pDA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ACharacter_Base::HeavyAttack);
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


	if (vInput.X != 0.f)
		GetCharacterMovement()->AddInputVector(GetActorForwardVector() * vInput.X);
	if (vInput.Y != 0.f)
		GetCharacterMovement()->AddInputVector(GetActorRightVector() * vInput.Y);
}

void ACharacter_Base::LightAttack(const FInputActionInstance& _Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Spear Character LightAttack"));

	if (!IsLightAttack) // 기본공격
	{
		IsLightAttack = true;
		bCanCombo = true;  // 나중에 노티파이로 콤보 가능 시간으로 빼기
		
		// 콤보 공격을 이을 수 있는 시간 설정 (0.5)
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ACharacter_Base::ResetCombo, ComboWindowTime, false);

		// 기본 공격 몽타주 재생 (첫 번째 공격 애니메이션 - 'LightAttack' 섹션)
		GetMesh()->GetAnimInstance()->Montage_Play(DefaultMontage.LoadSynchronous(), 1.0f);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("LightAttack"), DefaultMontage.LoadSynchronous());
	}
	else if (bCanCombo && ComboCount < 2)
	{
		ComboAttack();
	}
}

void ACharacter_Base::MiddleAttack(const FInputActionInstance& _Instance)
{
}

void ACharacter_Base::HeavyAttack(const FInputActionInstance& _Instance)
{
}


void ACharacter_Base::ComboAttack()
{
	// 콤보 카운트를 기반으로 콤보 공격 진행
	ComboCount++;
	if (ComboCount == 1)
	{		// 두 번째 공격 애니메이션으로 세팅
		GetMesh()->GetAnimInstance()->Montage_SetNextSection(FName("LightAttack"), FName("MiddleAttack"), DefaultMontage.LoadSynchronous());

	}
	else if (ComboCount == 2)
	{		
		// 세 번째 공격 애니메이션으로 세팅
		GetMesh()->GetAnimInstance()->Montage_SetNextSection(FName("MiddleAttack"), FName("HeavyAttack"), DefaultMontage.LoadSynchronous());

		// 콤보 끝, 리셋
		bCanCombo = false;
	}

	// 콤보가 계속될 수 있도록 윈도우 타이머 재설정 (0.5초)
	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ACharacter_Base::ResetCombo, ComboWindowTime, false);
}

void ACharacter_Base::ResetCombo()
{
	bCanCombo = false;
	// 콤보 타이머 취소
	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);
}
