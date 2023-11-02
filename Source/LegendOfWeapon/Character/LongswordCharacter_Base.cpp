// Fill out your copyright notice in the Description page of Project Settings.


#include "LongswordCharacter_Base.h"

ALongswordCharacter_Base::ALongswordCharacter_Base()
{	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ALongswordCharacter_Base::BeginPlay()
{
	Super::BeginPlay();
}

void ALongswordCharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALongswordCharacter_Base::LightAttack(const FInputActionInstance& _Instance)
{	
	// 해당 액션시 수행해야 할 코드 작성
	UE_LOG(LogTemp, Warning, TEXT("Longsword Character LightAttack"));
}

void ALongswordCharacter_Base::MiddleAttack(const FInputActionInstance& _Instance)
{
}

void ALongswordCharacter_Base::HeavyAttack(const FInputActionInstance& _Instance)
{
}
