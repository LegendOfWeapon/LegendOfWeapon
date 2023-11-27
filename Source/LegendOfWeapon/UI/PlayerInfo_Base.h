// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfo_Base.generated.h"

/**
 * 
 */
UCLASS()
class LEGENDOFWEAPON_API UPlayerInfo_Base : public UUserWidget
{
	GENERATED_BODY()

private:
	class UTextBlock* m_Name;
	class UProgressBar* m_HPBar;

public:
	void SetName(FString _Name);
	void SetHPBarRatio(float _ratio);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DT) override;
};
