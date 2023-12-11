// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/global.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GlobalDefaultServerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LEGENDOFWEAPON_API AGlobalDefaultServerGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AGlobalDefaultServerGameMode();

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
	FUniqueNetIdPtr GetPlayerNetId(APlayerController* player);
	void CreateSession();
	FOnlineSessionSettings* GetSessionInfo(FName SessionName);
	void ConnectPlayerToSession(APlayerController* player);
	FNamedOnlineSession* GetSession(FString SessionName);
	void JoinSession(APlayerController* playerController);

private:
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

private:
	IOnlineSessionPtr OnlineSessionInterface; //OnlineSession Interface

private:
	TArray<FPlayerInfo*> players; //플레이어 관리
	TQueue<FPlayerInfo*> sessionQ; //세션에 들어갈 대기열 관리
	TArray<FString> sessionNames; //세션 관리	
	TSharedRef<FOnlineSessionSearch> MySessionSearch; //Result Of Session Searching

private:
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;//세션 생성 델리게이트 바인딩
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;//세션 생성 델리게이트 바인딩
};
