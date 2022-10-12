// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UESurfCppGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UESURFCPP_API AUESurfCppGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "De")
	static bool LoadMesh(AActor *Actor);
	
};
