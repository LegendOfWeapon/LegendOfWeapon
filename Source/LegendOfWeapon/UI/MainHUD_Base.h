// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../Header/global.h"
#include "Blueprint/UserWidget.h"

#include "PlayerInfo_Base.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD_Base.generated.h"

/**
 * 
 */
UCLASS()
class LEGENDOFWEAPON_API UMainHUD_Base : public UUserWidget
{
	GENERATED_BODY()

private:
	UPlayerInfo_Base * m_PlayerInfo;


public:
	UPlayerInfo_Base* GetPlayerInfoWidget() { return m_PlayerInfo; }
	void SetPlayerHPRatio(float _Ratio);

	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DT) override;
};
