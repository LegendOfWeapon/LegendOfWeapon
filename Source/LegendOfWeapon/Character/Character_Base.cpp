// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Base.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
ACharacter_Base::ACharacter_Base()
	: m_Cam(nullptr)
	, m_Arm(nullptr)
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
			// LocalPlayer의 EnhancedInput 시스템을 얻어서, 사용할 IMC를 세팅
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

			//case EInputActionType::ATTACK:
			//	InputCom->BindAction(pDA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ACharacter_Base::Attack);
			//	break;

			}
		}
	}
}

void ACharacter_Base::Move(const FInputActionInstance& _Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Call Move Function!!!!"));

	FVector2D vInput = _Instance.GetValue().Get<FVector2D>();

	GetActorForwardVector()* vInput.X;
	GetActorForwardVector()* vInput.Y;


	if (vInput.X != 0.f)
		GetCharacterMovement()->AddInputVector(GetActorForwardVector() * vInput.X);
	if (vInput.Y != 0.f)
		GetCharacterMovement()->AddInputVector(GetActorRightVector() * vInput.Y);
}
