// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD_Base.h"


void UMainHUD_Base::NativeConstruct()
{
	Super::NativeConstruct();

	m_PlayerInfo = Cast<UPlayerInfo_Base>(GetWidgetFromName(FName("PlayerInfo")));
	if (!IsValid(m_PlayerInfo))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerInfo find Fail"));
	}

}

void UMainHUD_Base::NativeTick(const FGeometry& _geo, float _DT)
{
	Super::NativeTick(_geo, _DT);
}


void UMainHUD_Base::SetPlayerHPRatio(float _Ratio)
{
	m_PlayerInfo->SetHPBarRatio(_Ratio);
}
