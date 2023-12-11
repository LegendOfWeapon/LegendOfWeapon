// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalDefaultServerGameMode.h"
#include "../LegendOfWeapon.h"

#include "Net/UnrealNetwork.h"
#include "OnlineSubsystem.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

AGlobalDefaultServerGameMode::AGlobalDefaultServerGameMode()
	:MySessionSearch(MakeShareable(new FOnlineSessionSearch()))
{
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/LegendOfWeapon.MyPlayerController'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	OnFindSessionsCompleteDelegate.BindUObject(this, &AGlobalDefaultServerGameMode::OnFindSessionsComplete);
	CreateSessionCompleteDelegate.BindUObject(this, &AGlobalDefaultServerGameMode::OnCreateSessionComplete);

	GET_ONLINE_SESSIONS(OnlineSessions);
	OnlineSessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
	OnlineSessions->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
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
	
	FPlayerInfo* playerInfo = new FPlayerInfo;
	playerInfo->Controller = playerController;
	playerInfo->playerIndex = players.Num();
	playerInfo->sessionName = nullptr;
	players.Add(playerInfo);

	sessionQ.Enqueue(playerInfo);

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
			AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Session Create"), *GetName());
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

	if (sessionNames.Num() == 0) {
		CreateSession();
	}

	if (player != nullptr) {
		JoinSession(player);
	}
}

FNamedOnlineSession* AGlobalDefaultServerGameMode::GetSession(FString SessionName)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Ingage GetSession"), *GetName());
	GET_ONLINE_SESSIONS(OnlineSessions);
	IOnlineSessionPtr Sessions = OnlineSessions;

	FNamedOnlineSession* session = Sessions->GetNamedSession(FName(*SessionName));

	if (session != nullptr) {
		AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Session Belongs to"), *GetName());
		return session;
	}
	else {
		AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Session Belongs to"), *GetName());
		return nullptr;
	}

}

void AGlobalDefaultServerGameMode::JoinSession(APlayerController* playerController)
{
	GET_ONLINE_SESSIONS(OnlineSessions);
	IOnlineSessionPtr Sessions = OnlineSessions;

	MySessionSearch->bIsLanQuery = false;
	MySessionSearch->MaxSearchResults = 100;
	const FName SettingKeywords = TEXT("MyCustomKeywords");
	FString MySessionKeyword = TEXT("StartMap");
	MySessionSearch->QuerySettings.Set(SettingKeywords, MySessionKeyword, EOnlineComparisonOp::Equals);

	FUniqueNetIdPtr PlayerId = playerController->PlayerState->GetUniqueId().GetUniqueNetId();
	Sessions->FindSessions(*PlayerId, MySessionSearch);
	AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Player Find Session"), *GetName());
}

void AGlobalDefaultServerGameMode::OnFindSessionsComplete(bool bWasSuccessful)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());
	GET_ONLINE_SESSIONS(OnlineSessions);

	if (bWasSuccessful)
	{
		for (const FOnlineSessionSearchResult& SearchResult : MySessionSearch->SearchResults)
		{
			GetSession(sessionNames[sessionNames.Num() - 1]);
			if (SearchResult.Session.SessionSettings.Settings.Contains(SETTING_MAPNAME))
			{
				GetSession(sessionNames[sessionNames.Num() - 1]);
				FString FoundSessionName;
				SearchResult.Session.SessionSettings.Get(SETTING_MAPNAME, FoundSessionName);
				UE_LOG(LogTemp, Warning, TEXT("Session Name: %s"), *FoundSessionName);

				if (FoundSessionName == sessionNames[sessionNames.Num()-1]) // 여기서 "YourSessionName"을 원하는 세션 이름으로 대체
				{
					// 세션 참가
					AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Player Ingage Session"), *GetName());
					FPlayerInfo* playerController;
					sessionQ.Dequeue(playerController);
					FUniqueNetIdPtr PlayerId = playerController->Controller->PlayerState->GetUniqueId().GetUniqueNetId();
					OnlineSessions->JoinSession(*PlayerId,"StartSession", SearchResult);
					playerController->sessionName = sessionNames[sessionNames.Num() - 1];
					break;
				}
			}
		}
	}
}

void AGlobalDefaultServerGameMode::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful) {
		AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Session is created."), *GetName());
		UE_LOG(LogTemp, Log, TEXT("Log Message: %s"), *(SessionName.ToString()));
	}
	else {
		AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Session is not created"), *GetName());
	}
}
