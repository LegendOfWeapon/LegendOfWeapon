// Fill out your copyright notice in the Description page of Project Settings.


#include "SpearCharacter_Base.h"

ASpearCharacter_Base::ASpearCharacter_Base()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// â ĳ������ ����Ʈ(�����޺�) ��Ÿ�ָ� ã�Ƽ� �����Ѵ�.
	ConstructorHelpers::FObjectFinder<UAnimMontage> montage(TEXT("/Script/Engine.AnimMontage'/Game/BluePrints/Player/Spear/Animation/Montages/Spear_Default_Montage.Spear_Default_Montage'"));
	if (montage.Succeeded())
	{
		DefaultMontage = montage.Object;
	}
}

void ASpearCharacter_Base::BeginPlay()
{
	Super::BeginPlay();
}

void ASpearCharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpearCharacter_Base::LightAttack(const FInputActionInstance& _Instance)
{
	Super::LightAttack(_Instance);

	//// �ش� �׼ǽ� �����ؾ� �� �ڵ� �ۼ�
	//UE_LOG(LogTemp, Warning, TEXT("Spear Character LightAttack"));
}

void ASpearCharacter_Base::MiddleAttack(const FInputActionInstance& _Instance)
{
}

void ASpearCharacter_Base::HeavyAttack(const FInputActionInstance& _Instance)
{
}
