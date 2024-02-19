// Fill out your copyright notice in the Description page of Project Settings.
#include "Weapon_Base.h"


#include "../Character/Character_Base.h"

// Sets default values
AWeapon_Base::AWeapon_Base()
	:m_pOwner(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UBoxComponent* BoxCollision = FindComponentByClass<UBoxComponent>();
	if (BoxCollision)
	{
		BoxCollision->SetNotifyRigidBodyCollision(true);
		BoxCollision->OnComponentHit.AddDynamic(this, &AWeapon_Base::OnHit);
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon_Base::OnOverlapBegin);
		BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon_Base::OnOverlapEnd);
	}
}

// Called when the game starts or when spawned
void AWeapon_Base::BeginPlay()
{
	Super::BeginPlay();

	DamageAmount = 10.0f;
}

// Called every frame
void AWeapon_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon_Base::OnHit(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, FVector _vNormalImpulse, const FHitResult& _Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Weapon_Base OnHit!!@@@@@@@@@@@@@"));
}

void AWeapon_Base::OnOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TSubclassOf<UDamageType> DamageTypeClass;
	DamageTypeClass = UDamageType::StaticClass();
	UE_LOG(LogTemp, Warning, TEXT("Begin Over lap@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"));
	// Apply damage to the player
    if (OtherActor && OtherActor != GetOwner() && OtherActor->IsA(ACharacter_Base::StaticClass()))
    {
        APawn* OwnerPawn = Cast<APawn>(GetOwner());
        if (OwnerPawn)
        {
            AController* OwnerController = OwnerPawn->GetController();
            if (OwnerController)
            {
                // Apply damage to the player
                UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, OwnerController, this, DamageTypeClass);
				UE_LOG(LogTemp, Warning, TEXT("Weapon_Base Apply Damage@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"));
            }
        }
    }
}

void AWeapon_Base::OnOverlapEnd_Implementation(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index)
{
}

