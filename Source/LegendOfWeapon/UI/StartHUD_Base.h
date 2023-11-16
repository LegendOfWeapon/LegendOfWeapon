// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartHUD_Base.generated.h"

/**
 * 
 */
UCLASS()
class LEGENDOFWEAPON_API UStartHUD_Base : public UUserWidget
{
	GENERATED_BODY()

private:
	UButton* m_StartBtn;
	UButton* m_EndBtn;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DT) override;

public:
	UFUNCTION()
	void StartBtnClicked();

	UFUNCTION()
	void EndBtnClicked();
};
