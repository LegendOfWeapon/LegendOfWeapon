// Fill out your copyright notice in the Description page of Project Settings.


#include "StartHUD_Base.h"

#include "../Header/global.h"
#include "Blueprint/UserWidget.h"
#include "../Character/MyPlayerController.h"

void UStartHUD_Base::NativeConstruct()
{
	Super::NativeConstruct();

	m_StartBtn = Cast<UButton>(GetWidgetFromName(FName("Btn_start")));
	m_EndBtn = Cast<UButton>(GetWidgetFromName(FName("Btn_end")));

	if (!IsValid(m_StartBtn) || !IsValid(m_EndBtn))
	{
		UE_LOG(LogTemp, Error, TEXT("Button Reference failed"));
	}
	else
	{
		m_StartBtn->OnClicked.AddDynamic(this, &UStartHUD_Base::StartBtnClicked);

		m_EndBtn->OnClicked.AddDynamic(this, &UStartHUD_Base::EndBtnClicked);
	}
}

void UStartHUD_Base::NativeTick(const FGeometry& _geo, float _DT)
{
	Super::NativeTick(_geo, _DT);
}


void UStartHUD_Base::StartBtnClicked()
{
	UE_LOG(LogTemp, Error, TEXT("Start Button Clicked"));

	AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwningPlayer());
	if (PC)
	{
		PC->OpenLevelOnServer();
	}
}


void UStartHUD_Base::EndBtnClicked()
{
	UE_LOG(LogTemp, Error, TEXT("End Button Clicked"));

	UKismetSystemLibrary::QuitGame(GetWorld()
		, GetWorld()->GetFirstPlayerController()
		, EQuitPreference::Quit, true);
}
