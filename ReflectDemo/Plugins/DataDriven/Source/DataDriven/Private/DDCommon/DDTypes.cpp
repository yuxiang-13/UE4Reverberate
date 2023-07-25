// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCommon/DDTypes.h"

// 初始化静态成员变量
TSharedPtr<DDRecord> DDRecord::RecordInst = NULL;

TSharedPtr<DDRecord> DDRecord::Get()
{
	if (!RecordInst.IsValid())
	{
		RecordInst = MakeShareable(new DDRecord());
	}

	return RecordInst;
}



