// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDUserWidget.h"

void UDDUserWidget::DDRelease()
{
	IDDOO::DDRelease();

	// 在检测一次 从父UI移除
	RemoveFromParent();

	// 释放内存 从Root 移除
	RemoveFromRoot();
	// 准备回收
	ConditionalBeginDestroy();
}
