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
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	m_Arm->SetupAttachment(GetCapsuleComponent());
	m_Cam->SetupAttachment(m_Arm);
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

	if (!IsLightAttack) // �⺻����
	{
		IsLightAttack = true;
		bCanCombo = true;  // ���߿� ��Ƽ���̷� �޺� ���� �ð����� ����
		
		// �޺� ������ ���� �� �ִ� �ð� ���� (0.5)
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ACharacter_Base::ResetCombo, ComboWindowTime, false);

		// �⺻ ���� ��Ÿ�� ��� (ù ��° ���� �ִϸ��̼� - 'LightAttack' ����)
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
	// �޺� ī��Ʈ�� ������� �޺� ���� ����
	ComboCount++;
	if (ComboCount == 1)
	{		// �� ��° ���� �ִϸ��̼����� ����
		GetMesh()->GetAnimInstance()->Montage_SetNextSection(FName("LightAttack"), FName("MiddleAttack"), DefaultMontage.LoadSynchronous());

	}
	else if (ComboCount == 2)
	{		
		// �� ��° ���� �ִϸ��̼����� ����
		GetMesh()->GetAnimInstance()->Montage_SetNextSection(FName("MiddleAttack"), FName("HeavyAttack"), DefaultMontage.LoadSynchronous());

		// �޺� ��, ����
		bCanCombo = false;
	}

	// �޺��� ��ӵ� �� �ֵ��� ������ Ÿ�̸� �缳�� (0.5��)
	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ACharacter_Base::ResetCombo, ComboWindowTime, false);
}

void ACharacter_Base::ResetCombo()
{
	bCanCombo = false;
	// �޺� Ÿ�̸� ���
	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);
}
