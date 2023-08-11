// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCore/DDMessage.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off)
#endif


UDDInputBinder::UDDInputBinder()
{
	InputCount = 0;
	bExecuteWhenPause = false;
}

void UDDInputBinder::PressEvent()
{
	InputCount ++;
	// 如果 InputCount 与 TotalCount 相等，说明所有按键都按下了
	if (InputCount == TotalCount)
	{
		// 如果允许在暂停时执行
		if (bExecuteWhenPause)
		{
			InputDele.ExecuteIfBound();
		} else if (!bExecuteWhenPause && !UDDCommon::Get()->IsPauseGame())
		{
			InputDele.ExecuteIfBound();
		}
	}
}

void UDDInputBinder::ReleaseEvent()
{
	InputCount --;
}

UDDMessage::UDDMessage()
{
	MsgQuene = new DDMsgQuene();
}

void UDDMessage::MessageInit()
{

}

void UDDMessage::MessageBeginPlay()
{
	// 从UDDCommon获取PlayerCOntroller
	PlayerController = UDDCommon::Get()->GetController();
}

void UDDMessage::MessageTick(float DeltaSeconds)
{
	//处理协程
	TArray<FName> CompleteTask;
	for (TMap<FName, TMap<FName, DDCoroTask*>>::TIterator It(CoroStack); It; ++It)
	{
		TArray<FName> CompleteNode;
		for (TMap<FName, DDCoroTask*>::TIterator Ih(It->Value); Ih; ++Ih)
		{
			Ih->Value->Work(DeltaSeconds);
			if (Ih.Value()->IsDestroy || Ih->Value->IsFinish())
			{
				delete Ih->Value;
				CompleteNode.Push(Ih->Key);
			}
		}
		for (int i = 0; i < CompleteNode.Num(); ++i)
			It->Value.Remove(CompleteNode[i]);
		if (It->Value.Num() == 0)
			CompleteTask.Push(It->Key);
	}
	for (int i = 0; i < CompleteTask.Num(); ++i)
		CoroStack.Remove(CompleteTask[i]);

	// 处理延迟系统
	CompleteTask.Empty();
	for (TMap<FName, TMap<FName, DDInvokeTask*>>::TIterator It(InvokeStack); It; ++It)
	{
		TArray<FName> CompleteNode;
		for (TMap<FName, DDInvokeTask*>::TIterator Ih(It->Value); Ih; ++Ih)
		{
			if (Ih.Value()->IsDestroy || Ih->Value->UpdateOperate(DeltaSeconds))
			{
				delete Ih.Value();
				CompleteNode.Push(Ih->Key);
			}
		}
		for (int i = 0; i < CompleteNode.Num(); ++i)
		{
			It->Value.Remove(CompleteNode[i]);
		}
		if (It->Value.Num() == 0)
			CompleteTask.Push(It->Key);
	}
	for (int i = 0; i < CompleteTask.Num(); ++i)
		InvokeStack.Remove(CompleteTask[i]);
}

bool UDDMessage::StartCoroutine(FName ObjectName, FName CoroName, DDCoroTask* CoroTask)
{
	if (!CoroStack.Contains(ObjectName))
	{
		TMap<FName, DDCoroTask*> NewTaskStack;
		CoroStack.Add(ObjectName, NewTaskStack);
	}
	if (!(CoroStack.Find(ObjectName)->Contains(CoroName)))
	{
		CoroStack.Find(ObjectName)->Add(CoroName, CoroTask);
		return true;
	}
	delete CoroTask;
	return false;
}

bool UDDMessage::StopCoroutine(FName ObjectName, FName CoroName)
{
	if (CoroStack.Contains(ObjectName) && CoroStack.Find(ObjectName)->Find(CoroName))
	{
		DDCoroTask* CoroTask = *(CoroStack.Find(ObjectName)->Find(CoroName));
		CoroTask->IsDestroy = true;
		return true;
	}
	return false;
}

void UDDMessage::StopAllCorotine(FName ObjectName)
{
	if (CoroStack.Contains(ObjectName))
	{
		for (TMap<FName, DDCoroTask*>::TIterator It(*CoroStack.Find(ObjectName)); It; ++It)
			It->Value->IsDestroy = true;
	}
}




bool UDDMessage::StartInvoke(FName ObjectName, FName InvokeName, DDInvokeTask* InvokeTask)
{
	if (!InvokeStack.Contains(ObjectName))
	{
		TMap<FName, DDInvokeTask*> NewTaskStack;
		InvokeStack.Add(ObjectName, NewTaskStack);
	}
	if (!(InvokeStack.Find(ObjectName)->Contains(InvokeName)))
	{
		InvokeStack.Find(ObjectName)->Add(InvokeName, InvokeTask);
		return true;
	}
	delete InvokeTask;
	return false;
}

bool UDDMessage::StopInvoke(FName ObjectName, FName InvokeName)
{
	if (InvokeStack.Contains(ObjectName) && InvokeStack.Find(ObjectName)->Find(InvokeName))
	{
		DDInvokeTask* InvokeTask = *(InvokeStack.Find(ObjectName)->Find(InvokeName));
		InvokeTask->IsDestroy = true;
		return true;
	}
	return false;
}

void UDDMessage::StopAllInvoke(FName ObjectName)
{
	if (InvokeStack.Contains(ObjectName))
	{
		for (TMap<FName, DDInvokeTask*>::TIterator It(*InvokeStack.Find(ObjectName)); It; ++It)
			It->Value->IsDestroy = true;
	}
}

void UDDMessage::UnBindInput(FName ObjectName)
{
	if (!BinderGroup.Contains(ObjectName))
	{
		return;
	}
	TArray<UDDInputBinder*> BinderList = *BinderGroup.Find(ObjectName);
	
	for (int i = BinderList.Num() - 1; i >= 0; --i)
	{
		// 解除根
		BinderList[i]->RemoveFromRoot();
		// 开始销毁
		BinderList[i]->ConditionalBeginDestroy();
	}
	BinderGroup.Remove(ObjectName);
}

#if PLATFORM_WINDOWS
#pragma optimize("",on)
#endif