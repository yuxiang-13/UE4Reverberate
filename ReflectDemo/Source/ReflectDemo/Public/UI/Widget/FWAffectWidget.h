// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FWAffectWidget.generated.h"

class UImage;
class UOverlay; // 这个是根，在蓝图上看出来的
class UButton;
/**
 * 
 */
UCLASS()
class REFLECTDEMO_API UFWAffectWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 第三种获取方式，反射绑定  (命名一致)
	UPROPERTY(meta=(BindWidget))
	UButton* ButtonOne;
	
public:
	// 构造
	UFWAffectWidget(const FObjectInitializer& ObjectInitializer);

	// 初始化
	virtual bool Initialize() override;

	// 让蓝图中Button的点击事件调用
	UFUNCTION(BlueprintCallable, Category="FrameWork")
	void ButtonThreeEvent();

	UFUNCTION()
	void ButtonOneEvent();
	UFUNCTION()
	void ButtonTwoEvent();
	
protected:
	UImage* BGImage;
	UOverlay* RootPanel;

	// 动态图片
	UImage* DynImage;
};
