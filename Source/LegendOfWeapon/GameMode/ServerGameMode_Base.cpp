// Fill out your copyright notice in the Description page of Project Settings.


#include "./ServerGameMode_Base.h"

#include "../LegendOfWeapon.h"
#include "./ServerGameState_Base.h"

AServerGameMode_Base::AServerGameMode_Base()
{
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/LegendOfWeapon.ServerPlayerController_Base'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	GameStateClass = AServerGameState_Base::StaticClass();
}

void AServerGameMode_Base::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(MyTimerHandle, this, &AServerGameMode_Base::DamagePlayers, 1.0f, true, 10.0f);
}

void AServerGameMode_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AServerGameMode_Base::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("==========================================================="));
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	//ErrorMessage = TEXT("Server is full");

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

APlayerController* AServerGameMode_Base::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));

	return NewPlayerController;
}

void AServerGameMode_Base::PostLogin(APlayerController* newPlayer)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostLogin(newPlayer);

	UNetDriver* NetDriver = GetNetDriver();
	if (NetDriver)
	{
		for (const auto& Connection : NetDriver->ClientConnections)
		{
			AB_LOG(LogABNetwork, Log, TEXT("Client Connections : %s"), *Connection->GetName());
		}
	}
	else
	{
		AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
	}

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));

	FPlayerInfo playerInfo;
	playerInfo.Controller = newPlayer;
	playerInfo.playerIndex = PlayerInfoList.Num();
	PlayerInfoList.Add(playerInfo);
	UE_LOG(LogTemp, Warning, TEXT("PlayerLogin : %d"), playerInfo.playerIndex);

	if (PlayerInfoList.Num() == 2) {
		UE_LOG(LogTemp, Warning, TEXT("StartPlaying"));
		PlayerCharacterPossesing(EWeaponType::LONGSWORD, EWeaponType::SPEAR);
	}
}

void AServerGameMode_Base::StartPlay()
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::StartPlay();

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

TArray<int8> AServerGameMode_Base::GetPlayerIndices()
{
	TArray<int8> indices;

	for (const FPlayerInfo& playerInfo : PlayerInfoList) {
		if (playerInfo.Controller != nullptr) {
			indices.Add(playerInfo.Controller->NetPlayerIndex);
		}
		else {
			//PlayerControllerList.Remove(playerController);
			UE_LOG(LogTemp, Warning, TEXT("delete playerController"));
		}
	}

	return indices;
}

void AServerGameMode_Base::PlayerCharacterPossesing(EWeaponType player1, EWeaponType player2)
{
	PlayerInfoList[0].WeaponType = player1;

	UClass* pawnClass = nullptr;
	switch (player1)
	{
	case EWeaponType::NONE:
		break;
	case EWeaponType::LONGSWORD:
	{
		pawnClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrints/Player/Longsword/BPC_Longsword_Player.BPC_Longsword_Player_C'"));
		if (pawnClass == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("PawnSelecting is failed"));
			ensure(false);
		}
	}
	break;
	case EWeaponType::SPEAR:
	{
		pawnClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrints/Player/Spear/Character/BPC_Spear_Player.BPC_Spear_Player_C'"));
		if (pawnClass == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("PawnSelecting is failed"));
			ensure(false);
		}
	}
	break;
	case EWeaponType::END:
		break;
	default:
		break;
	}

	APawn* newPawn = nullptr;

	if (pawnClass != nullptr) {
		newPawn = GetWorld()->SpawnActor<APawn>(pawnClass, PlayerInfoList[0].Controller->GetSpawnLocation(), PlayerInfoList[0].Controller->GetControlRotation());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("PawnSelecting is failed"));
		ensure(false);
	}

	if (newPawn != nullptr) {
		PlayerInfoList[0].Controller->Possess(newPawn);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("PawnSelecting is failed"));
		ensure(false);
	}

	////////////////////////////////////////////////////////

	PlayerInfoList[1].WeaponType = player2;

	pawnClass = nullptr;
	switch (player2)
	{
	case EWeaponType::NONE:
		ensure(false);
		break;
	case EWeaponType::LONGSWORD:
	{
		pawnClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrints/Player/Longsword/BPC_Longsword_Player.BPC_Longsword_Player_C'"));
		if (pawnClass == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("PawnSelecting is failed"));
			ensure(false);
		}
	}
	break;
	case EWeaponType::SPEAR:
	{
		pawnClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrints/Player/Spear/Character/BPC_Spear_Player.BPC_Spear_Player_C'"));
		if (pawnClass == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("PawnSelecting is failed"));
			ensure(false);
		}
	}
	break;
	case EWeaponType::END:
		ensure(false);
		break;
	default:
		break;
	}
	newPawn = GetWorld()->SpawnActor<APawn>(pawnClass, PlayerInfoList[1].Controller->GetSpawnLocation(), PlayerInfoList[1].Controller->GetControlRotation());

	if (newPawn != nullptr) {
		PlayerInfoList[1].Controller->Possess(newPawn);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("PawnSelecting is failed"));
		ensure(false);
	}
}

void AServerGameMode_Base::DamagePlayers()
{
	UE_LOG(LogTemp, Warning, TEXT("Function of DamagePlayers is called"));
	for (int i = 0; i < PlayerInfoList.Num(); i++) {
		if (PlayerInfoList[i].Controller) {
			AActor* playerActor = PlayerInfoList[i].Controller->GetPawn();
			if (playerActor) {
				UGameplayStatics::ApplyDamage(playerActor, 10.0f, nullptr, nullptr, NULL);
			}
		}
	}
}