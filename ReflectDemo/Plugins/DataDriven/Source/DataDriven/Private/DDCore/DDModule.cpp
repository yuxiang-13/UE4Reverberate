// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCore/DDModule.h"

#include "DDCore/DDModel.h"
#include "DDCore/DDWealth.h"
#include "DDObject/DDOO.h"

// Sets default values for this component's properties
UDDModule::UDDModule()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UDDModule::CreateManager()
{
	// 创建三个模块   自定义的UObject类，NewObject<UDDModel>() 参数不用传This
	
	// 数据模块 指针
	Model = NewObject<UDDModel>();
	// 消息模块 指针
	Message = NewObject<UDDMessage>();
	// 资源模块 指针
	Wealth = NewObject<UDDWealth>();

	// 避免被垃圾回收销毁
	Model->AddToRoot();
	Message->AddToRoot();
	Wealth->AddToRoot();

	// 指定模组
	Model->AssignModule(this);
	Message->AssignModule(this);
	Wealth->AssignModule(this);
}

void UDDModule::ModuleInit()
{
	Model->ModelInit();
	Message->MessageInit();
	Wealth->WealthInit();

}

void UDDModule::ModuleBeginPlay()
{
	Model->ModelBeginPlay();
	Message->MessageBeginPlay();
	Wealth->WealthBeginPlay();
}

void UDDModule::ModuleTick(float DeltaSeconds)
{
	Model->ModelTick(DeltaSeconds);
	Message->MessageTick(DeltaSeconds);
	Wealth->WealthTick(DeltaSeconds);
}

void UDDModule::ChangeModuleType(FName ModuleType)
{
	ModuleIndex = DDH::GetEnumIndexFromName(ModuleType.ToString(), GetFName());

	if (ModuleIndex < 0)
	{
		//DDH::Debug(10) << "生成模具错误----" << GetName() << DDH::Endl();
	}
}

void UDDModule::RegisterObject(IDDOO* ObjectInst)
{
	// 注册对象到数据模块
	Model->RegisterObject(ObjectInst);
	// 把此 模组自己  注册到对象数组
	ObjectInst->AssignModule(this);
}

void UDDModule::ChildDestroy(FName ObjectName)
{
	Model->DestroyObject(ObjectName);
}




void UDDModule::DestroyObject(EAgreementType Agreement, TArray<FName> TargetNameGroup)
{
	Model->DestroyObject(Agreement, TargetNameGroup);
}

void UDDModule::EnableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup)
{
	Model->EnableObject(Agreement, TargetNameGroup);
}

void UDDModule::DisableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup)
{
	Model->DisableObject(Agreement, TargetNameGroup);
}


void UDDModule::ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param)
{
	// 调用Module的UFunction
	UFunction* ExeFunc = this->FindFunction(Agreement.FunctionName);
	if (ExeFunc)
	{
		// 设置调用成功
		Param->CallResult = ECallResult::Succeed;
		// 调用方法
		ProcessEvent(ExeFunc, Param->ParamPtr);
	} else
	{
		// 设置方法不存在
		Param->CallResult = ECallResult::NoFunction;
	}
}

void UDDModule::ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param)
{
	//区分类型执行放射方法
	switch (Agreement.AgreementType)
	{
	case EAgreementType::SelfObject:
		ExecuteSelfObject(Agreement, Param);
		break;
	case EAgreementType::OtherObject:
		ExecuteOtherObject(Agreement, Param);
		break;
	case EAgreementType::ClassOtherObject:
		ExecuteClassOtherObject(Agreement, Param);
		break;
	case EAgreementType::SelfClass:
		ExecuteSelfClass(Agreement, Param);
		break;
	case EAgreementType::OtherClass:
		ExecuteOtherClass(Agreement, Param);
		break;
	case EAgreementType::All:
		ExecuteAll(Agreement, Param);
		break;
	}
}

bool UDDModule::StartCoroution(FName ObjectName, FName CoroName, DDCoroTask* CoroTask)
{
	return Message->StartCoroution(ObjectName, CoroName, CoroTask);
}

bool UDDModule::StopCoroution(FName ObjectName, FName CoroName)
{
	return Message->StopCoroution(ObjectName, CoroName);
}

void UDDModule::StopAllCorotion(FName ObjectName)
{
	return Message->StopAllCorotion(ObjectName);
}

void UDDModule::ExecuteSelfObject(DDObjectAgreement Agreement, DDParam* Param)
{
	// 定义存储目标对象的组
	TArray<IDDOO*> TargetObjectGroup;
	// 从数据模组获取对象组
	Model->GetSelfObject(Agreement.ObjectGroup, TargetObjectGroup);
	// 循环 调用反射事件
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		// 获取 反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			// 设置 调用成功
			Param->CallResult = ECallResult::Succeed;
			// 执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		} else
		{
			// 设置 找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
	// 如果获取的对象有缺失，设置结果为对象缺失，缺失是设置为优先级最高，哪怕上面设置成了ECallResult::Succeed
	if (TargetObjectGroup.Num() != Agreement.ObjectGroup.Num())
	{
		Param->CallResult = ECallResult::LackObject;
	}
}

void UDDModule::ExecuteOtherObject(DDObjectAgreement Agreement, DDParam* Param)
{
	// 定义存储目标对象的组
	TArray<IDDOO*> TargetObjectGroup;
	// 从数据模组获取对象组
	int32 TotalObjectNum = Model->GetOtherObject(Agreement.ObjectGroup, TargetObjectGroup);
	// 循环 调用反射事件
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		// 获取 反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			// 设置 调用成功
			Param->CallResult = ECallResult::Succeed;
			// 执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		} else
		{
			// 设置 找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}

	// 判断对象是否存在缺失问题
	if (Agreement.ObjectGroup.Num() + TargetObjectGroup.Num() != TotalObjectNum)
	{
		Param->CallResult = ECallResult::LackObject;
	}
}

void UDDModule::ExecuteClassOtherObject(DDObjectAgreement Agreement, DDParam* Param)
{
	// 定义存储目标对象的组
	TArray<IDDOO*> TargetObjectGroup;
	// 从数据模组获取对象组
	int32 TotalClassNum = Model->GetClassOtherObject(Agreement.ObjectGroup, TargetObjectGroup);
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		// 获取 反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			// 设置 调用成功
			Param->CallResult = ECallResult::Succeed;
			// 执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		} else
		{
			// 设置 找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
	// 判断对象缺失
	// 判断对象是否存在缺失问题
	if (Agreement.ObjectGroup.Num() + TargetObjectGroup.Num() != TotalClassNum)
	{
		Param->CallResult = ECallResult::LackObject;
	}
}

void UDDModule::ExecuteSelfClass(DDObjectAgreement Agreement, DDParam* Param)
{
	// 定义存储目标对象的组
	TArray<IDDOO*> TargetObjectGroup;
	// 从数据模组获取对象组
	Model->GetSelfClass(Agreement.ObjectGroup, TargetObjectGroup);
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		// 获取 反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			// 设置 调用成功
			Param->CallResult = ECallResult::Succeed;
			// 执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		} else
		{
			// 设置 找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
	// 判断对象缺失
	if (TargetObjectGroup.Num() == 0)
	{
		Param->CallResult = ECallResult::LackObject;
	}
}

void UDDModule::ExecuteOtherClass(DDObjectAgreement Agreement, DDParam* Param)
{
	// 定义存储目标对象的组
	TArray<IDDOO*> TargetObjectGroup;
	// 从数据模组获取对象组
	Model->GetOtherClass(Agreement.ObjectGroup, TargetObjectGroup);
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		// 获取 反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			// 设置 调用成功
			Param->CallResult = ECallResult::Succeed;
			// 执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		} else
		{
			// 设置 找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
	// 判断对象缺失
	if (TargetObjectGroup.Num() == 0)
	{
		Param->CallResult = ECallResult::LackObject;
	}
}

void UDDModule::ExecuteAll(DDObjectAgreement Agreement, DDParam* Param)
{
	// 定义存储目标对象的组
	TArray<IDDOO*> TargetObjectGroup;
	// 从数据模组获取对象组
	Model->GetAll(TargetObjectGroup);
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		// 获取 反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			// 设置 调用成功
			Param->CallResult = ECallResult::Succeed;
			// 执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		} else
		{
			// 设置 找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
}

