// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfo_Base.h"


#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UPlayerInfo_Base::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_Name = Cast<UTextBlock>(GetWidgetFromName(FName("Name")));
	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(FName("HPBar")));

}

void UPlayerInfo_Base::NativeTick(const FGeometry& _geo, float _DT)
{
	Super::NativeTick(_geo, _DT);
}

void UPlayerInfo_Base::SetName(FString _Name)
{
	m_Name->SetText(FText::FromString(_Name));
}

void UPlayerInfo_Base::SetHPBarRatio(float _ratio)
{
	m_HPBar->SetPercent(_ratio);
}
