// Fill out your copyright notice in the Description page of Project Settings.


#include "Wealth/WealthWidget.h"

#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Kismet/GameplayStatics.h"
#include "Wealth/WealthActor.h"

bool UWealthWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	TArray<AActor*> ActArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWealthActor::StaticClass(), ActArray);
	if (ActArray.Num() > 0)
	{
		AWealthActor* WealthActor = Cast<AWealthActor>(ActArray[0]);
		WealthActor->AssignWealthWidget(this);
	}
	
	return true;
}

void UWealthWidget::AssignTexture(UTexture2D* InTexture)
{
	TexImage->SetBrushFromTexture(InTexture);
}

void UWealthWidget::AssignContent(UUserWidget* InWidget)
{
	UOverlaySlot* ContentSlot = ContentBox->AddChildToOverlay(InWidget);
	ContentSlot->SetPadding(FMargin(0.f));
	ContentSlot->SetHorizontalAlignment(HAlign_Fill);
	ContentSlot->SetVerticalAlignment(VAlign_Fill);
	
}
