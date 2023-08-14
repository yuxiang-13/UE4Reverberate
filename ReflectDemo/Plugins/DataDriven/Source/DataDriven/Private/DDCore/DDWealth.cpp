// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCore/DDWealth.h"

#include "Blueprint/UserWidget.h"
#include "DDObject/DDOO.h"


struct ObjectSingleLoadNode
{
	//加载句柄
	TSharedPtr<FStreamableHandle> WealthHandle;
	//资源结构体
	FObjectWealthEntry* WealthEntry;
	//请求对象名
	FName ObjectName;
	//回调方法名
	FName FunName;
	//构造函数
	ObjectSingleLoadNode(TSharedPtr<FStreamableHandle> InWealthHandle, FObjectWealthEntry* InWealthEntry, FName InObjectName, FName InFunName)
	{
		WealthHandle = InWealthHandle;
		WealthEntry = InWealthEntry;
		ObjectName = InObjectName;
		FunName = InFunName;
	}
};

struct ObjectKindLoadNode
{
	// 加载句柄
	TSharedPtr<FStreamableHandle> WealthHandle;
	// 没有加载资源的数组
	TArray<FObjectWealthEntry*> UnLoadWealthEntry;
	// 已经加载资源的数组
	TArray<FObjectWealthEntry*> LoadWealthEntry;
	//请求对象名
	FName ObjectName;
	//回调方法名
	FName FunName;
	
	ObjectKindLoadNode(TSharedPtr<FStreamableHandle> InWealthHandle, TArray<FObjectWealthEntry*>& InUnLoadWealthEntry, TArray<FObjectWealthEntry*>& InLoadWealthEntry, FName InObjectName, FName InFunName)
	{
		WealthHandle = InWealthHandle;
		UnLoadWealthEntry = InUnLoadWealthEntry;
		LoadWealthEntry = InLoadWealthEntry;
		ObjectName = InObjectName;
		FunName = InFunName;
	}
};


void UDDWealth::WealthInit()
{
}

void UDDWealth::WealthBeginPlay()
{
	// 遍历自动生成对象
	for (int i = 0; i < WealthData.Num(); ++i)
	{
		// 生成Object对象
		for (int j = 0; j < WealthData[i]->AutoObjectData.Num(); ++j)
		{
			// 根据获取到的Class生成指定的 Object
			UObject* NewObj = NewObject<UObject>(this, WealthData[i]->AutoObjectData[j].WealthClass);
			// 防止垃圾回收
			NewObj->AddToRoot();
			IDDOO* InstPtr = Cast<IDDOO>(NewObj);
			// 是本框架对象
			if (InstPtr)
			{
				InstPtr->RegisterToModule(
					WealthData[i]->ModuleName.IsNone() ? IModule->GetFName() : WealthData[i]->ModuleName,
					WealthData[i]->AutoObjectData[j].ObjectName,
					WealthData[i]->AutoObjectData[j].ClassName
				);
			}
		}
		// 生成Actor对象
		for (int j = 0; j < WealthData[i]->AutoActorData.Num(); ++j)
		{
			AActor* NewAct = GetDDWorld()->SpawnActor<AActor>(
				WealthData[i]->AutoActorData[j].WealthClass,
				WealthData[i]->AutoActorData[j].Transform
			);
			IDDOO* InstPtr = Cast<IDDOO>(NewAct);
			// 是本框架对象
			if (InstPtr)
			{
				InstPtr->RegisterToModule(
					WealthData[i]->ModuleName.IsNone() ? IModule->GetFName() : WealthData[i]->ModuleName,
					WealthData[i]->AutoActorData[j].ObjectName,
					WealthData[i]->AutoActorData[j].ClassName
				);
			}
		}
		// 生成Widget对象
		for (int j = 0; j < WealthData[i]->AutoWidgetData.Num(); ++j)
		{
			UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetDDWorld(), WealthData[i]->AutoWidgetData[j].WealthClass);
			// 防止回收 [通过 UPROPERTY 保护]， 另外，UserWidget 特殊，不能通过 AddToRoot 防止垃圾回收
			GCWidgetGroup.Push(NewWidget);
			
			IDDOO* InstPtr = Cast<IDDOO>(NewWidget);
			// 是本框架对象
			if (InstPtr)
			{
				InstPtr->RegisterToModule(
					WealthData[i]->ModuleName.IsNone() ? IModule->GetFName() : WealthData[i]->ModuleName,
					WealthData[i]->AutoWidgetData[j].ObjectName,
					WealthData[i]->AutoWidgetData[j].ClassName
				);
			}
		}
	}
}

void UDDWealth::WealthTick(float DeltaSeconds)
{
	DealObjectSingleLoadStack();
	DealObjectKindLoadStack();
}

void UDDWealth::AssignData(TArray<UWealthData*>& InWealthData)
{
	WealthData = InWealthData;
}

FWealthURL* UDDWealth::GainWealthURL(FName WealthName)
{
	for (int i = 0; i < WealthData.Num(); ++i)
	{
		for (int j = 0; j < WealthData[i]->WealthURL.Num(); ++j)
		{
			if (WealthData[i]->WealthURL[j].WealthName.IsEqual(WealthName))
			{
				return &WealthData[i]->WealthURL[j];
			}
		}
	}
	return NULL;
}

void UDDWealth::GainWealthURL(FName WealthKind, TArray<FWealthURL*>& OutURL)
{
	for (int i = 0; i < WealthData.Num(); ++i)
	{
		for (int j = 0; j < WealthData[i]->WealthURL.Num(); ++j)
		{
			if (WealthData[i]->WealthURL[j].WealthKind.IsEqual(WealthKind))
			{
				OutURL.Push(&WealthData[i]->WealthURL[j]);
			}
		}
	}
}

void UDDWealth::LoadObjectWealth(FName WealthName, FName ObjectName, FName FunName)
{
	// 获取缓存中的资源结构体
	FObjectWealthEntry* WealthEntry = GetObjectSingleEntry(WealthName);
	// 如果为空
	if (!WealthEntry)
	{
		DDH::Debug(10) << ObjectName << " Get Null Wealth " << WealthName << DDH::Endl();
		return;
	}
	// 如果资源不可用
	if (!WealthEntry->WealthPath.IsValid())
	{
		DDH::Debug(10) << ObjectName << " Get UnVaild Wealth " << WealthName << DDH::Endl();
		return;
	}
	// 如果资源已经加载
	if (WealthEntry->WealthObject)
	{
		// 直接返回 已经存在的资源对象
		//         int32 ModuleIndex|-|EAgreementType AgreementType|-|FName ObjName|-|FName FunctionName|-|ParamType1 ParamName1
		BackObjectWealth(ModuleIndex, ObjectName, FunName, WealthName, WealthEntry->WealthObject);
	} else
	{
		// 异步加载
		TSharedPtr<FStreamableHandle> WealthHandle = WealthLoader.RequestAsyncLoad(WealthEntry->WealthPath);
		// 添加新节点到加载序列
		ObjectSingleLoadStack.Push(new ObjectSingleLoadNode(WealthHandle, WealthEntry, ObjectName, FunName));
	}
}

void UDDWealth::LoadObjectWealthKind(FName WealthKind, FName ObjectName, FName FunName)
{
	TArray<FObjectWealthEntry*> WealthEntryGroup = GetObjectKindEntry(WealthKind);
	// 如果数量为0
	if (WealthEntryGroup.Num() == 0)
	{
		DDH::Debug(10) << ObjectName << " Get Null WealthKind " << WealthKind << DDH::Endl();
		return;
	}
	// 资源可不可用
	for (int i = 0; i < WealthEntryGroup.Num(); ++i)
	{
		if (!WealthEntryGroup[i]->WealthPath.IsValid())
		{
			DDH::Debug(10) << ObjectName << " Get Not Vaild in Kind :  " << WealthKind << " For Name :   " << WealthEntryGroup[i]->WealthName << DDH::Endl();
			return;
		}
	}
	// 还没有加载的资源
	TArray<FObjectWealthEntry*> UnLoadWealthEntry;
	// 已经加载的资源
	TArray<FObjectWealthEntry*> LoadWealthEntry;
	// 资源加载与否分类
	for (int i = 0; i < WealthEntryGroup.Num(); ++i)
	{
		if (WealthEntryGroup[i]->WealthObject)
		{
			LoadWealthEntry.Push(WealthEntryGroup[i]);
		} else
		{
			UnLoadWealthEntry.Push(WealthEntryGroup[i]);
		}
	}
	// 如果未加载资源为0
	if (UnLoadWealthEntry.Num() == 0)
	{
		// 直接返回这些所有的已经加载好的资源
		TArray<FName> NameGroup;
		TArray<UObject*> WealthGroup;
		for (int i = 0; i < LoadWealthEntry.Num(); ++i)
		{
			NameGroup.Push(LoadWealthEntry[i]->WealthName);
			WealthGroup.Push(LoadWealthEntry[i]->WealthObject);
		}
		BackObjectWealthKind(ModuleIndex, ObjectName, FunName, NameGroup, WealthGroup);
	} else
	{
		// 开始异步批量加载
		TArray<FSoftObjectPath> WealthPaths;
		for (int i = 0; i < UnLoadWealthEntry.Num(); ++i)
		{
			WealthPaths.Push(UnLoadWealthEntry[i]->WealthPath);
		}
		// 开始 异步加载
		TSharedPtr<FStreamableHandle> WealthHandle = WealthLoader.RequestAsyncLoad(WealthPaths);
		
		//生成加载节点
		ObjectKindLoadStack.Push(new ObjectKindLoadNode(WealthHandle, UnLoadWealthEntry, LoadWealthEntry, ObjectName, FunName));
	}
}


void UDDWealth::DealObjectKindLoadStack()
{
	// 定义加载完成的序列
	TArray<ObjectKindLoadNode*> CompleteStack;
	for (int i = 0; i < ObjectKindLoadStack.Num(); ++i)
	{
		// 是否已经加载完成
		if (ObjectKindLoadStack[i]->WealthHandle->HasLoadCompleted())
		{
			// 直接返回这些所有的已经加载好的资源
			TArray<FName> NameGroup;
			TArray<UObject*> WealthGroup;
			
			// 1 填充已经加载好的资源
			for (int j = 0; j < ObjectKindLoadStack[i]->LoadWealthEntry.Num(); ++j)
			{
				NameGroup.Push(ObjectKindLoadStack[i]->LoadWealthEntry[j]->WealthName);
				WealthGroup.Push(ObjectKindLoadStack[i]->LoadWealthEntry[j]->WealthObject);
			}
			
			// 2 遍历设置所有未加载 资源结构体为已加载
			for (int j = 0; j < ObjectKindLoadStack[i]->UnLoadWealthEntry.Num(); ++j)
			{
				ObjectKindLoadStack[i]->UnLoadWealthEntry[j]->WealthObject = ObjectKindLoadStack[i]->UnLoadWealthEntry[j]->WealthPath.ResolveObject();
				// 填充已加载好的资源
				NameGroup.Push(ObjectKindLoadStack[i]->UnLoadWealthEntry[j]->WealthName);
				WealthGroup.Push(ObjectKindLoadStack[i]->UnLoadWealthEntry[j]->WealthObject);
			}
			// 返回数据
			BackObjectWealthKind(ModuleIndex, ObjectKindLoadStack[i]->ObjectName, ObjectKindLoadStack[i]->FunName, NameGroup, WealthGroup);
			// 添加节点到已完成序列
			CompleteStack.Push(ObjectKindLoadStack[i]);
		}
	}
	// 销毁已经完成的节点
	for (int i = 0; i < CompleteStack.Num(); ++i)
	{
		// 移除节点序列
		ObjectKindLoadStack.Remove(CompleteStack[i]);
		delete CompleteStack[i];
	}
}



FObjectWealthEntry* UDDWealth::GetObjectSingleEntry(FName WealthName)
{
	for (int i = 0; i < WealthData.Num(); ++i)
	{
		for (int j = 0; j < WealthData[i]->ObjectWealthData.Num(); ++j)
		{
			if (WealthData[i]->ObjectWealthData[j].WealthName.IsEqual(WealthName))
			{
				return &(WealthData[i]->ObjectWealthData[j]);
			}
		}
	}
	return NULL;
}

TArray<FObjectWealthEntry*> UDDWealth::GetObjectKindEntry(FName WealthKind)
{
	TArray<FObjectWealthEntry*> WealthGroup;
	for (int i = 0; i < WealthData.Num(); ++i)
	{
		for (int j = 0; j < WealthData[i]->ObjectWealthData.Num(); ++j)
		{
			if (WealthData[i]->ObjectWealthData[j].WealthKind.IsEqual(WealthKind))
			{
				WealthGroup.Push(&(WealthData[i]->ObjectWealthData[j]));
			}
		}
	}
	return WealthGroup;
}

void UDDWealth::DealObjectSingleLoadStack()
{
	// 定义加载完成的序列
	TArray<ObjectSingleLoadNode*> CompleteStack;
	for (int i = 0; i < ObjectSingleLoadStack.Num(); ++i)
	{
		// 判断是否已经加载完成
		if (ObjectSingleLoadStack[i]->WealthHandle->HasLoadCompleted())
		{
			// 设置对应资源完成
			ObjectSingleLoadStack[i]->WealthEntry->WealthObject = ObjectSingleLoadStack[i]->WealthEntry->WealthPath.ResolveObject();
			// 返回资源给对象
			BackObjectWealth(
				ModuleIndex,
				ObjectSingleLoadStack[i]->ObjectName,
				ObjectSingleLoadStack[i]->FunName,
				ObjectSingleLoadStack[i]->WealthEntry->WealthName,
				ObjectSingleLoadStack[i]->WealthEntry->WealthObject
			);
			// 添加已经加载完成的节点到临时序列
			CompleteStack.Push(ObjectSingleLoadStack[i]);
		}
	}
	// 销毁已经完成的节点
	for (int i = 0; i < CompleteStack.Num(); ++i)
	{
		// 移除节点序列
		ObjectSingleLoadStack.Remove(CompleteStack[i]);
		delete CompleteStack[i];
	}
}