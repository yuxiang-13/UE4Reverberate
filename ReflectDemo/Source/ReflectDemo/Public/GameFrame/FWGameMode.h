// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FWGameMode.generated.h"

/**
 * 
 */
UCLASS()
class REFLECTDEMO_API AFWGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AFWGameMode();

	virtual void BeginPlay() override;
};
