// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalDefaultServerGameMode.h"
#include "../LegendOfWeapon.h"

#include "Net/UnrealNetwork.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

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
	
	//FPlayerInfo playerInfo;
	//playerInfo.Controller = playerController;
	//playerInfo.playerIndex = players.Num();
	//playerInfo.sessionName = nullptr;
	//players.Add(playerInfo);

	return playerController;
}

void AGlobalDefaultServerGameMode::PostLogin(APlayerController* newPlayer)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());
	Super::PostLogin(newPlayer);
	AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("End"), *GetName());

	ConnectPlayerToSession(newPlayer);
}

void AGlobalDefaultServerGameMode::StartPlay()
{
	Super::StartPlay();
	
	CreateSession();	
}

FUniqueNetIdPtr AGlobalDefaultServerGameMode::GetPlayerNetId(APlayerController* player)
{
	if (player != nullptr) {
		FUniqueNetIdPtr playerId = player->PlayerState->GetUniqueId().GetUniqueNetId();
		return playerId;
	}
	else {
		return nullptr;
	}
}

void AGlobalDefaultServerGameMode::CreateSession()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			FString sessionName = sessionNames.Num() + "Session";
			//sessionNames.Add(FName(*sessionName));
			sessionNames.Add(sessionName);

			FOnlineSessionSettings SessionSettings;
			SessionSettings.bIsLANMatch = false;
			SessionSettings.NumPublicConnections = 2; // 최대 연결 가능 플레이어 수
			SessionSettings.bShouldAdvertise = true;

			Sessions->CreateSession(0, FName(*sessionNames[sessionNames.Num() - 1]), SessionSettings);
		}
	}
}

FOnlineSessionSettings* AGlobalDefaultServerGameMode::GetSessionInfo(FName SessionName)
{
	GET_ONLINE_SESSIONS(OnlineSessions);
	IOnlineSessionPtr Sessions = OnlineSessions;

	FOnlineSessionSettings* SessionSettings = Sessions->GetSessionSettings(SessionName);
	if (SessionSettings != nullptr) {
		return SessionSettings;
	}
	else {
		return nullptr;
	}
}


void AGlobalDefaultServerGameMode::ConnectPlayerToSession(APlayerController* player)
{
	GET_ONLINE_SESSIONS(OnlineSessions);

	if (player != nullptr) {
		auto PlayerId = player->PlayerState->GetUniqueId().GetUniqueNetId();
		FNamedOnlineSession* session = GetSession(sessionNames[sessionNames.Num() - 1]);
	}
}

FNamedOnlineSession* AGlobalDefaultServerGameMode::GetSession(FString SessionName)
{
	GET_ONLINE_SESSIONS(OnlineSessions);
	IOnlineSessionPtr Sessions = OnlineSessions;

	FNamedOnlineSession* session = Sessions->GetNamedSession(FName(*SessionName));

	return session;
}

void AGlobalDefaultServerGameMode::JoinSession(APlayerController* playerController)
{
	GET_ONLINE_SESSIONS(OnlineSessions);
	IOnlineSessionPtr Sessions = OnlineSessions;

	MySessionSearch->bIsLanQuery = false;
	MySessionSearch->MaxSearchResults = 100;
	MySessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	//FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	//OnFindSessionsCompleteDelegate.BindUObject(this, &AGlobalDefaultServerGameMode::OnFindSessionsComplete);
	
	//FUniqueNetIdPtr PlayerId = playerController->PlayerState->GetUniqueId().GetUniqueNetId();
	//Sessions->FindSessions(*PlayerId, MySessionSearch);
}

void AGlobalDefaultServerGameMode::OnFindSessionsComplete(bool bWasSuccessful)
{
}

//void AGlobalDefaultServerGameMode::OnFindSessionsComplete(bool bWasSuccessful)
//{
//	GET_ONLINE_SESSIONS(OnlineSessions);
//
//	if (bWasSuccessful)
//	{
//		for (const FOnlineSessionSearchResult& SearchResult : MySessionSearch->SearchResults)
//		{
//			if (SearchResult.Session.SessionSettings.Settings.Contains(SETTING_MAPNAME))
//			{
//				FString FoundSessionName;
//				SearchResult.Session.SessionSettings.Get(SETTING_MAPNAME, FoundSessionName);
//
//				if (FoundSessionName == sessionNames[sessionNames.Num()-1]) // 여기서 "YourSessionName"을 원하는 세션 이름으로 대체
//				{
//					// 세션 참가
//					OnlineSessions->JoinSession(0, "StartSession", SearchResult);
//					break;
//				}
//			}
//		}
//	}
//}
