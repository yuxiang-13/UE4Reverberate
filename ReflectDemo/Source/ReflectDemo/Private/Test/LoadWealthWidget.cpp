// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/LoadWealthWidget.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"


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

	// StartCoroutine("BuildWidgetTest", BuildWidgetTest());
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

void ULoadWealthWidget::BuildSingleWidget(FName BackName, UUserWidget* BackWidget)
{
	DDH::Debug(10) << "BuildSingleWidget : " << BackName << DDH::Endl();
	SizeBox_1->ClearChildren();
	SizeBox_1->AddChild(BackWidget);
}

void ULoadWealthWidget::BuildKindWidget(TArray<FName> BackNames, TArray<UUserWidget*> BackWidgets)
{
	for (int i = 0; i < BackNames.Num(); ++i)
	{
		DDH::Debug(10) << "BuildKindWidget : " << BackNames[i] << DDH::Endl();
	}
	SizeBox_1->ClearChildren();
	SizeBox_2->ClearChildren();
	SizeBox_3->ClearChildren();
	SizeBox_1->AddChild(BackWidgets[0]);
	SizeBox_2->AddChild(BackWidgets[1]);
	SizeBox_3->AddChild(BackWidgets[2]);
}

void ULoadWealthWidget::BuildMultiWidget(FName BackName, TArray<UUserWidget*> BackWidgets)
{
	DDH::Debug(10) << "BuildMultiWidget : " << BackName << DDH::Endl();
	SizeBox_1->ClearChildren();
	SizeBox_2->ClearChildren();
	SizeBox_3->ClearChildren();
	SizeBox_1->AddChild(BackWidgets[0]);
	SizeBox_2->AddChild(BackWidgets[1]);
	SizeBox_3->AddChild(BackWidgets[2]);
}

DDCoroTask* ULoadWealthWidget::BuildWidgetTest()
{
	DDCORO_PARAM(ULoadWealthWidget)
#include DDCORO_BEGIN()
	D->BuildSingleClassWealth(EWealthType::Widget, "ViewWidget2", "BuildSingleWidget");
#include DDYIELD_READY()
	DDYIELD_RETURN_SECOND(10.f)
	D->BuildKindClassWealth(EWealthType::Widget, "ViewWidget", "BuildKindWidget");

#include DDYIELD_READY()
	DDYIELD_RETURN_SECOND(10.f)
	D->BuildMultiClassWealth(EWealthType::Widget, "ViewWidget3", 3, "BuildMultiWidget");
	
#include DDCORO_END() 
}
