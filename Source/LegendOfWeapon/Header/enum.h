#pragma once

UENUM(BlueprintType)
enum class EInputActionType : uint8
{
	MOVE,
	LIGHT_ATTACK,
	MIDDLE_ATTACK,
	HEAVY_ATTACK,
	BLOCK,
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
	LightAttack,
	MiddleAttack,
	HeavyAttack,
	W_LightAttack UMETA(DisplayName = "W Light Attack"),
	A_LightAttack UMETA(DisplayName = "A Light Attack"),
	S_LightAttack UMETA(DisplayName = "S Light Attack"),
	D_LightAttack UMETA(DisplayName = "D Light Attack"),
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle,
	Disabled,
	Attacking,
	Blocking,
	Dead,
};