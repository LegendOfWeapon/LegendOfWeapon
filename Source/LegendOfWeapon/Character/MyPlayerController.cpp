// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "../LegendOfWeapon.h"
#include "../GameMode/StarterGameMode_Base.h"


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
	if (IsLocalController())
	{
		AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Client"));
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("ServerTestMap"));
	}

	if (GetLocalRole() == ROLE_Authority) {
		AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Server"));
		//UGameplayStatics::OpenLevel(GetWorld(), TEXT("ServerTestMap"));
		
		//this->ClientTravel(TEXT("ServerTestMap"), ETravelType::TRAVEL_Absolute);

		//FString NewLevelName = "/Game/Maps/ServerTestMap";
		//FString GameModeName = "ServerGameMode_Base";
		//
		//FString TravelURL = FString::Printf(TEXT("%s?game=%s"), *NewLevelName, *GameModeName);
		//this->PreClientTravel(TravelURL, ETravelType::TRAVEL_Absolute, true);
		//this->ClientRestart(nullptr);
		
		GetWorld()->ServerTravel(TEXT("ServerTestMap"));
	}
}


void AMyPlayerController::ClientTravelWithServer()
{
	/*UWorld* world = GetWorld();
	if (world) {
		world->ServerTravel(TEXT("ServerTestMap"));
	}*/

	this->ClientTravel(TEXT("ServerTestMap"), ETravelType::TRAVEL_Absolute);
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

