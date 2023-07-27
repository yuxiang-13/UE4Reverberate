// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDObject.h"

void UDDObject::DDRelease()
{
	IDDOO::DDRelease();

	// 释放内存 从Root 移除
	RemoveFromRoot();
	// 准备回收
	ConditionalBeginDestroy();
	
}
