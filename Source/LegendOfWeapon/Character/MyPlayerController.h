// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../UI/StartHUD_Base.h"
#include "../Header/global.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 * ���۸ʿ��� �������Ӹ����� �̵��ϱ� ���� �����ϴ� �÷��̾� ��Ʈ�ѷ��Դϴ�. ����ȯ�� ������ �������� �̷�� ���� ����
 */
UCLASS()
class LEGENDOFWEAPON_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	TSubclassOf<UStartHUD_Base>	m_StartHUDClass;
	UStartHUD_Base* m_StartHUD;


public:
	// Ŭ���̾�Ʈ�� ȣ���ϰ� �������� ����Ǵ� �Լ� ����
	UFUNCTION(Server, Reliable)
	void ServerOpenLevel();

	// �� �Լ��� Ŭ���̾�Ʈ�� ȣ���մϴ�
	void OpenLevelOnServer();

	virtual void BeginPlay() override;

public:
	AMyPlayerController();
	~AMyPlayerController();


};