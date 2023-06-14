// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/FWHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/FWHUDWidget.h"

void AFWHUD::BeginPlay()
{
	Super::BeginPlay();

	// 创建Widget方法一
	UFWHUDWidget* HUDWidget = CreateWidget<UFWHUDWidget>(GetWorld(), HUDWidgetClass);
	HUDWidget->AddToViewport();
}
