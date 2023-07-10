// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WealthWidget.generated.h"


class UImage;
class UOverlay;
/**
 * 
 */
UCLASS()
class REFLECTDEMO_API UWealthWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(Meta = (BindWidget))
	UImage* TexImage;
	UPROPERTY(Meta = (BindWidget))
	UOverlay* ContentBox;
public:
	// Widget初始化函数
	virtual bool Initialize() override;

	void AssignTexture(UTexture2D* InTexture);

	void AssignContent(UUserWidget* InWidget);
};
