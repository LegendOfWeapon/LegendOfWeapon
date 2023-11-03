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