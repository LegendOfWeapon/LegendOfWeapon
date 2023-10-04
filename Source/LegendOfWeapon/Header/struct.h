#pragma once

#include "enum.h"
#include "InputAction.h"

#include "Engine/DataTable.h"
#include "Struct.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FIAData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInputActionType				Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UInputAction>	Action;
};
