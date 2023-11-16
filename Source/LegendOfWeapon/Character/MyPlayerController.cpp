// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"


AMyPlayerController::AMyPlayerController()
{
	ConstructorHelpers::FClassFinder<UUserWidget> StartHUDFind(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrints/UI/StartHUD.StartHUD_C'"));

	if (StartHUDFind.Succeeded())
	{
		m_StartHUDClass = StartHUDFind.Class;
	}
}

AMyPlayerController::~AMyPlayerController()
{
}


void AMyPlayerController::ServerOpenLevel_Implementation()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("ServerTestMap"));
}


void AMyPlayerController::OpenLevelOnServer()
{
	if (IsLocalController())
	{
		ServerOpenLevel();
	}
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		UStartHUD_Base* StartHUD = CreateWidget<UStartHUD_Base>(this, m_StartHUDClass);
		if (StartHUD != nullptr)
		{
			StartHUD->AddToViewport();
		}
	}
}

