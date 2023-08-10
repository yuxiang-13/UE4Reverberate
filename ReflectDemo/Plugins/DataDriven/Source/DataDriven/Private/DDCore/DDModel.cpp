// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCore/DDModel.h"

#include "DDObject/DDOO.h"

void UDDModel::ModelInit()
{
}

void UDDModel::ModelBeginPlay()
{
}

void UDDModel::RegisterObject(IDDOO* ObjectInst)
{
	//如果不重复就添加到模组对象
	if (!ObjectGroup.Contains(ObjectInst->GetObjectName()))
	{
		// 1 添加到对象组
		ObjectGroup.Add(ObjectInst->GetObjectName(), ObjectInst);
		// 2 添加到对象类组
		FName ObjectClassName = ObjectInst->GetClassName();
		if (ObjectClassGroup.Contains(ObjectClassName))
		{
			ObjectClassGroup.Find(ObjectClassName)->Push(ObjectInst);
		} else
		{
			TArray<IDDOO*> ObjectArray;
			ObjectClassGroup.Add(ObjectClassName, ObjectArray);

			ObjectClassGroup.Find(ObjectClassName)->Push(ObjectInst);
		}
		// 3 添加对象到激活生命周期组
		ObjectActiveGroup.Push(ObjectInst);
		
	} else {
		// 有重复注册，就打印
		//DDH::Debug(10) << " Object Repeated ---> " << ObjectInst->GetObjectName() << DDH::Endl();
	}
}

void UDDModel::DestroyObject(FName ObjectName)
{
	// 获取需要倍销毁的对象
	if (ObjectGroup.Contains(ObjectName))
	{
		IDDOO* TargetObject = *ObjectGroup.Find(ObjectName);
		// 如果 销毁对象 以及 与销毁对象都没有该对象
		if (!ObjectDestroyGroup.Contains(TargetObject) && !PreObjectDestroyGroup.Contains(TargetObject))
		{
			// 根据生命周期状态，看看放到这两个数组中的哪一个
			//如果是稳定状态就添加到ObjectDestroyGroup,如果是激活状态就添加到PreObjectDestroyGroup
			switch (TargetObject->RunState)
			{
			case EBaseObjectState::Active: // 当前是 激活状态
				PreObjectDestroyGroup.Push(TargetObject);
				break;
			case EBaseObjectState::Stable: // 当前是 稳定状态  说明是运行完了生命周期函数
				ObjectDestroyGroup.Push(TargetObject);
				ObjectTickGroup.Remove(TargetObject);
				break;
			}
		}
	}
}

void UDDModel::ModelTick(float DeltaSeconds)
{
	// 运行Tick组的tick函数
	for (int i = 0; i < ObjectTickGroup.Num(); ++i)
	{
		ObjectTickGroup[i]->DDTick(DeltaSeconds);
	}
	
	// 保存 临时完成某个周期的对象
	TArray<IDDOO*> TempObjectGroup;
	// 循环运行激活生命周期函数
	for (int i = 0; i < ObjectActiveGroup.Num(); ++i)
	{
		// 如果激活成功
		if (ObjectActiveGroup[i]->ActiveLife())
		{
			TempObjectGroup.Push(ObjectActiveGroup[i]);
		}
	}
	
	//将运行完生命周期的对象移出生命周期组,并且将运行Tick运行的对象添加到Tick组
	for (int i = 0; i < TempObjectGroup.Num(); ++i)
	{
		ObjectActiveGroup.Remove(TempObjectGroup[i]);
		if (TempObjectGroup[i]->IsAllowTickEvent)
		{
			// 让下一帧去运行Tick函数
			ObjectTickGroup.Push(TempObjectGroup[i]);
		}
	}

	
	//执行释放对象函数, 清空释放组
	for (int i = 0; i < ObjectReleaseGroup.Num(); ++i)
	{
		ObjectReleaseGroup[i]->DDRelease();
	}
	ObjectReleaseGroup.Empty();

	
	//清空临时对象组， 存放要被删除的 对象
	TempObjectGroup.Empty();
	//运行销毁对象组的销毁生命周期函数
	for (int i = 0; i < ObjectDestroyGroup.Num(); ++i)
	{
		// 销毁时的各种 生命周期
		if (ObjectDestroyGroup[i]->DestroyLife())
		{
			// 添加对象到释放对象组
			ObjectReleaseGroup.Push(ObjectDestroyGroup[i]);

			//添加要被删除的 对象 到临时组
			TempObjectGroup.Push(ObjectDestroyGroup[i]);
			
			// 清除掉该被 销毁的对象
			ObjectGroup.Remove(ObjectDestroyGroup[i]->GetObjectName());
			ObjectClassGroup.Find(ObjectDestroyGroup[i]->GetClassName())->Remove(ObjectDestroyGroup[i]);
			// 如果类对象数组为空
			if (ObjectClassGroup.Find(ObjectDestroyGroup[i]->GetClassName())->Num() == 0)
				ObjectClassGroup.Remove(ObjectDestroyGroup[i]->GetClassName());
		}
	}
	//把销毁的对象从销毁对象组移除
	for (int i = 0; i < TempObjectGroup.Num(); ++i)
	{
		// 要被释放的对象， 剔除出 销毁组
		ObjectDestroyGroup.Remove(ObjectReleaseGroup[i]);
	}
	

	//清空临时对象组,用于 存放需要被销毁的对象
	TempObjectGroup.Empty();
	//处理预销毁对象组, 如果对象进入稳定状态, 就转到临时对象组
	for (int i = 0; i < PreObjectDestroyGroup.Num(); ++i)
	{
		if (PreObjectDestroyGroup[i]->RunState == EBaseObjectState::Stable)
		{
			//添加稳定运行状态的对象到临时对象组
			TempObjectGroup.Push(PreObjectDestroyGroup[i]);
		}
	}
	//从预销毁对象组中清除稳定状态对象
	for (int i = 0; i < TempObjectGroup.Num(); ++i)
	{
		PreObjectDestroyGroup.Remove(TempObjectGroup[i]);
		//添加到销毁对象组
		ObjectDestroyGroup.Push(TempObjectGroup[i]);
		//移除出帧函数组
		ObjectTickGroup.Remove(TempObjectGroup[i]);
	}

}

// 批量销毁
void UDDModel::DestroyObject(EAgreementType Agreement, TArray<FName> TargetNameGroup)
{
	// 获取到对应协议的对象数组
	TArray<IDDOO*> TargetObjectGroup;
	GetAgreementObject(Agreement, TargetNameGroup, TargetObjectGroup);

	// 批量销毁对象
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		// 如果 销毁对象 以及 与销毁对象都没有该对象
		if (!ObjectDestroyGroup.Contains(TargetObjectGroup[i]) && !PreObjectDestroyGroup.Contains(TargetObjectGroup[i]))
		{
			// 根据生命周期状态，看看放到这两个数组中的哪一个
			//如果是稳定状态就添加到ObjectDestroyGroup,如果是激活状态就添加到PreObjectDestroyGroup
			switch (TargetObjectGroup[i]->RunState)
			{
			case EBaseObjectState::Active: // 当前是 激活状态
				PreObjectDestroyGroup.Push(TargetObjectGroup[i]);
				break;
			case EBaseObjectState::Stable: // 当前是 稳定状态  说明是运行完了生命周期函数
				ObjectDestroyGroup.Push(TargetObjectGroup[i]);
				ObjectTickGroup.Remove(TargetObjectGroup[i]);
				break;
			}
		}
	}
}

void UDDModel::EnableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup)
{
	// 获取到对应协议的对象数组
	TArray<IDDOO*> TargetObjectGroup;
	GetAgreementObject(Agreement, TargetNameGroup, TargetObjectGroup);

	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		// 如果这个对象处于 稳定运行状态 + 生命状态为失活，那就能变为激活状态
		if (TargetObjectGroup[i]->RunState == EBaseObjectState::Stable && TargetObjectGroup[i]->LifeState == EBaseObjectLife::Disable)
		{
			TargetObjectGroup[i]->OnEnable();
		}
	}
	
}

void UDDModel::DisableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup)
{
	// 获取到对应协议的对象数组
	TArray<IDDOO*> TargetObjectGroup;
	GetAgreementObject(Agreement, TargetNameGroup, TargetObjectGroup);

	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		// 如果这个对象处于 稳定运行状态 + 生命状态为失活，那就能变为激活状态
		if (TargetObjectGroup[i]->RunState == EBaseObjectState::Stable && TargetObjectGroup[i]->LifeState == EBaseObjectLife::Enable)
		{
			TargetObjectGroup[i]->OnDisable();
		}
	}
}




void UDDModel::GetSelfObject(TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup)
{
	for (int i = 0; i < TargetNameGroup.Num(); ++i)
	{
		// 在模块中的 所有被注册Object中，查找
		if (ObjectGroup.Contains(TargetNameGroup[i]))
		{
			TargetObjectGroup.Push(* ObjectGroup.Find(TargetNameGroup[i]));
		}
	}
}

int32 UDDModel::GetOtherObject(TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup)
{
	for(TMap<FName, IDDOO*>::TIterator It(ObjectGroup); It; ++It)
	{
		bool IsSame = false;
		// 当前组的名字，不在 剔除目标数组，就可以添加
		for (int i = 0; i < TargetNameGroup.Num(); ++i)
		{
			// 检测名字，相同就退出
			if (TargetNameGroup[i].IsEqual(It->Key))
			{
				IsSame = true;
				break;
			}
		}
		if (!IsSame)
		{
			TargetObjectGroup.Push(It->Value);
		}
	}
	// 返回全部对象数量
	return ObjectGroup.Num();
}

int32 UDDModel::GetClassOtherObject(TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup)
{
	if (!ObjectGroup.Contains(TargetNameGroup[0])) return 0;
	
	// 获取对象名字
	FName ObjectClassName = (*ObjectGroup.Find(TargetNameGroup[0]))->GetClassName();
	// 迭代对象类实例的数组
	for (TArray<IDDOO*>::TIterator It(* ObjectClassGroup.Find(ObjectClassName)); It; ++It)
	{
		bool IsSame = false;
		for (int i = 0; i < TargetNameGroup.Num(); ++i)
		{
			if ((*It)->GetObjectName().IsEqual(TargetNameGroup[i]))
			{
				IsSame = true;
				break;
			}
		}
		if (!IsSame)
		{
			TargetObjectGroup.Push(*It);
		}
	}

	// 返回对象类型数组的数量
	return (* ObjectClassGroup.Find(ObjectClassName)).Num();
}

void UDDModel::GetSelfClass(TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup)
{
	for (int i = 0; i < TargetNameGroup.Num(); ++i)
	{
		// 如果不包换 这个类，就跳到下一个循环
		if (!ObjectClassGroup.Contains(TargetNameGroup[i]))
		{
			continue;
		} else
		{
			TargetObjectGroup.Append(* ObjectClassGroup.Find(TargetNameGroup[i]));
		}
	}
}

void UDDModel::GetOtherClass(TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup)
{
	for (TMap<FName, TArray<IDDOO*>>::TIterator It(ObjectClassGroup); It; ++It)
	{
		if (!TargetNameGroup.Contains(It->Key))
		{
			TargetObjectGroup.Append(It->Value);
		}
	}
}

void UDDModel::GetAll(TArray<IDDOO*>& TargetObjectGroup)
{
	ObjectGroup.GenerateValueArray(TargetObjectGroup);
}

void UDDModel::GetAgreementObject(EAgreementType Agreement, TArray<FName> TargetNameGroup,
	TArray<IDDOO*>& TargetObjectGroup)
{
	switch (Agreement) {
		case EAgreementType::SelfObject:
			GetSelfObject(TargetNameGroup, TargetObjectGroup);
			break;
		case EAgreementType::OtherObject:
			GetOtherObject(TargetNameGroup, TargetObjectGroup);
			break;
		case EAgreementType::ClassOtherObject:
			GetClassOtherObject(TargetNameGroup, TargetObjectGroup);
			break;
		case EAgreementType::SelfClass:
			GetSelfClass(TargetNameGroup, TargetObjectGroup);
			break;
		case EAgreementType::OtherClass:
			GetOtherClass(TargetNameGroup, TargetObjectGroup);
			break;
		case EAgreementType::All:
			GetAll(TargetObjectGroup);
			break;
		default:;
	}
}
