// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalDefaultServerGameMode.h"

#include "../LegendOfWeapon.h"

AGlobalDefaultServerGameMode::AGlobalDefaultServerGameMode()
{
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/LegendOfWeapon.MyPlayerController'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}

void AGlobalDefaultServerGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AGlobalDefaultServerGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGlobalDefaultServerGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

APlayerController* AGlobalDefaultServerGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* playerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	return playerController;
}

void AGlobalDefaultServerGameMode::PostLogin(APlayerController* newPlayer)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());
	Super::PostLogin(newPlayer);
	AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("End"), *GetName());
}

void AGlobalDefaultServerGameMode::StartPlay()
{
	Super::StartPlay();
}
