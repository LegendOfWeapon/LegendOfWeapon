// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/global.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ServerGameMode_Base.generated.h"

/**
 * 
 */
UCLASS()
class LEGENDOFWEAPON_API AServerGameMode_Base : public AGameModeBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AServerGameMode_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* newPlayer);
	virtual void StartPlay() override;

public:
	TArray<int8> GetPlayerIndices();
	void PlayerCharacterPossesing(EWeaponType player1, EWeaponType player2);

private:
	TArray<FPlayerInfo> PlayerInfoList;

private://temp variables
	FTimerHandle MyTimerHandle;

public://temp functions
	void DamagePlayers();
};
