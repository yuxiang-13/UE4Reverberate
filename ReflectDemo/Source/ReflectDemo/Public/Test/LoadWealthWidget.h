// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDObject/DDUserWidget.h"
#include "LoadWealthWidget.generated.h"


class UImage;
class UTexture2D;

UCLASS()
class REFLECTDEMO_API ULoadWealthWidget : public UDDUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(Meta=(BindWidget))
	UImage* ViewImage;

	int32 ImageIndex;
	
	TArray<UTexture2D*> TextureGroup;

	void ChangeImage();
public:
	virtual void DDInit() override;
	virtual void DDLoading() override;
	
	UFUNCTION()
	void LoadSingleTexture(FName BackName, UObject* BackWealth);

	// 跟哪个反射参数一致   DDOBJFUNC_TWO(BackObjectWealthKind, TArray<FName>, BackNames, TArray<UObject*>, BackWealths);
	UFUNCTION()
	void LoadKindTexture(TArray<FName> BackNames, TArray<UObject*> BackWealths);
};
