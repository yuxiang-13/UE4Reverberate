// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FWHUD.generated.h"

/**
 * 
 */
UCLASS()
class REFLECTDEMO_API AFWHUD : public AHUD
{
	GENERATED_BODY()

public:
	// 传 需要被实例化的 蓝图类
	UPROPERTY(EditAnywhere, Category="FrameWork")
	TSubclassOf<class UFWHUDWidget> HUDWidgetClass;

protected:
	virtual void BeginPlay() override;
	
};
