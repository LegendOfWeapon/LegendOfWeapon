// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerGameMode_Base.h"

AServerGameMode_Base::AServerGameMode_Base()
{
}

void AServerGameMode_Base::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void AServerGameMode_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AServerGameMode_Base::PostLogin(APlayerController* newPlayer)
{
	Super::PostLogin(newPlayer);

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
