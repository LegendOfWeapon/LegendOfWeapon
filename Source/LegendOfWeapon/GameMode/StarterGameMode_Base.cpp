// Fill out your copyright notice in the Description page of Project Settings.


#include "StarterGameMode_Base.h"

AStarterGameMode_Base::AStarterGameMode_Base()
{
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/LegendOfWeapon.MyPlayerController'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}

void AStarterGameMode_Base::BeginPlay()
{
	Super::BeginPlay();
}

void AStarterGameMode_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStarterGameMode_Base::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

APlayerController* AStarterGameMode_Base::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* playerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	return playerController;
}

void AStarterGameMode_Base::PostLogin(APlayerController* newPlayer)
{
	Super::PostLogin(newPlayer);
}

void AStarterGameMode_Base::StartPlay()
{
	Super::StartPlay();
}
