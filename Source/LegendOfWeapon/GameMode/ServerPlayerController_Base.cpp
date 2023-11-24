// Fill out your copyright notice in the Description page of Project Settings.


#include "./ServerPlayerController_Base.h"

#include "Blueprint/UserWidget.h"
#include "../UI/MainHUD_Base.h"

#include "../LegendOfWeapon.h"

AServerPlayerController_Base::AServerPlayerController_Base()
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainHUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrints/UI/MainHUD.MainHUD_C'"));

	if (MainHUD.Succeeded())
	{
		m_MainHudClass = MainHUD.Class;
	}

}

void AServerPlayerController_Base::PostInitializeComponents()
{
	AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());

	Super::PostInitializeComponents();

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AServerPlayerController_Base::PostNetInit()
{
	AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());

	Super::PostNetInit();

	UNetDriver* NetDriver = GetNetDriver();
	if (NetDriver)
	{
		AB_LOG(LogABNetwork, Log, TEXT("Server Connection : %s"), *NetDriver->ServerConnection->GetName());
	}
	else
	{
		AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
	}

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AServerPlayerController_Base::BeginPlay()
{
	AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());

	Super::BeginPlay();

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));

	// MainHUD 불러오기
	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevelName = World->GetMapName();
		CurrentLevelName.RemoveFromStart(World->StreamingLevelsPrefix); // 접두사 제거

		if (CurrentLevelName == "ServerTestMap")
		{
			if (IsValid(m_MainHudClass)&&IsLocalController())
			{
				m_MainHUD = Cast<UMainHUD_Base>(CreateWidget(this, m_MainHudClass));
				if (!IsValid(m_MainHUD))
				{
					UE_LOG(LogTemp, Error, TEXT("MainHUD_Base Fail"));
				}
				else
				{
					m_MainHUD->AddToViewport();
				}
			}
		}
	}
}

void AServerPlayerController_Base::OnPossess(APawn* InPawn)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());

	Super::OnPossess(InPawn);

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}
