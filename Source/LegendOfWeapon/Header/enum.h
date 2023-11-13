#pragma once

UENUM(BlueprintType)
enum class EInputActionType : uint8
{
	MOVE,
	LIGHT_ATTACK,
	MIDDLE_ATTACK,
	HEAVY_ATTACK,
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	NONE,

	LONGSWORD,
	SPEAR,

	END,
};

UENUM(BlueprintType)
enum class EAttackTypes : uint8
{
	LightAttack UMETA(DisplayName = "LightAttack"),
	MiddleAttack UMETA(DisplayName = "MiddleAttack"),
	HeavyAttack UMETA(DisplayName = "HeavyAttack"),
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle UMETA(DisplayName = "idle"),
	Disabled UMETA(DisplayName = "Disabled"),
	Attacking UMETA(DisplayName = "Attacking"),
	Blocking UMETA(DisplayName = "Blocking"),
	Dead UMETA(DisplayName = "Dead"),
};