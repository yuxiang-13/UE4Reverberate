// Fill out your copyright notice in the Description page of Project Settings.


#include "Wealth/TopWealthWidget.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Kismet/GameplayStatics.h"
#include "Wealth/TopWealthActor.h"

bool UTopWealthWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	TArray<AActor*> ActArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATopWealthActor::StaticClass(), ActArray);
	if (ActArray.Num() > 0)
	{
	}
	
	return true;
}

void UTopWealthWidget::AssignTexture(UTexture2D* InTexture)
{
}

void UTopWealthWidget::AssignContent(UUserWidget* InWidget)
{
	UOverlaySlot* ContentSlot = ContentBox->AddChildToOverlay(InWidget);
	ContentSlot->SetPadding(FMargin(0.f));
	ContentSlot->SetHorizontalAlignment(HAlign_Fill);
	ContentSlot->SetVerticalAlignment(VAlign_Fill);
	
}
