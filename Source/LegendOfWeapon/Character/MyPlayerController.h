// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../UI/StartHUD_Base.h"
#include "../Header/global.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 * 시작맵에서 서버게임맵으로 이동하기 위해 구현하던 플레이어 컨트롤러입니다. 맵전환의 구현을 나중으로 미루며 추후 보완
 */
UCLASS()
class LEGENDOFWEAPON_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	TSubclassOf<UStartHUD_Base>	m_StartHUDClass;
	UStartHUD_Base* m_StartHUD;


public:
	// 클라이언트가 호출하고 서버에서 실행되는 함수 선언
	UFUNCTION(Server, Reliable)
	void ServerOpenLevel();

	// 이 함수는 클라이언트가 호출합니다
	void OpenLevelOnServer();

	virtual void BeginPlay() override;

public:
	AMyPlayerController();
	~AMyPlayerController();


};