// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/LoadWealthWidget.h"

#include "Components/Image.h"


void ULoadWealthWidget::DDInit()
{
	Super::DDInit();
	AddToViewport(0);
}

void ULoadWealthWidget::DDLoading()
{
	Super::DDLoading();
	// LoadObjectWealth("ViewImage1", "LoadSingleTexture");

	// 1 资源名 2 回调函数名
	// 这样就能在 ViewImage 所有资源加载完成后，自动调用 LoadKindTexture 函数，把资源传到回调
	// LoadObjectWealthKind("ViewImage", "LoadKindTexture");
}

void ULoadWealthWidget::LoadSingleTexture(FName BackName, UObject* BackWealth)
{
	ViewImage->SetBrushFromTexture(Cast<UTexture2D>(BackWealth));
}

void ULoadWealthWidget::LoadKindTexture(TArray<FName> BackNames, TArray<UObject*> BackWealths)
{
	for (int i = 0; i < BackWealths.Num(); ++i)
	{
		DDH::Debug(10) << BackNames[i] << DDH::Endl();

		TextureGroup.Push(Cast<UTexture2D>(BackWealths[i]));
	}

	ImageIndex = 0;
	// 自定义调度器
	InvokeRepeat("ChangeImage", 1.f, 1.f, this, &ULoadWealthWidget::ChangeImage);
}

void ULoadWealthWidget::ChangeImage()
{
	ViewImage->SetBrushFromTexture(TextureGroup[ImageIndex]);
	ImageIndex = ImageIndex + 1 >= TextureGroup.Num() ? 0 : ImageIndex + 1;
}