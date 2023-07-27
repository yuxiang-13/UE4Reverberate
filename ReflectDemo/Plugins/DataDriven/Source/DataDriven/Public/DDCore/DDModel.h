// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDMM.h"
#include "UObject/NoExportTypes.h"
#include "DDModel.generated.h"


class IDDOO;

UCLASS()
class DATADRIVEN_API UDDModel : public UObject, public IDDMM
{
	GENERATED_BODY()
public:
	// 模块生命周期函数
	virtual void ModelInit();
	virtual void ModelBeginPlay();
	virtual void ModelTick(float DeltaSeconds);

	// 注册对象 到  数据模块
	void RegisterObject(IDDOO* ObjectInst);

	// 销毁对象，让销毁对象的本体 进行调用
	void DestroyObject(FName ObjectName);

	// 销毁对象 (对一群操作 -> 销毁一群小兵)
	void DestroyObject(EAgreementType Agreement, TArray<FName> TargetNameGroup);
	// 激活对象 (对一群操作 -> 激活一群小兵)
	void EnableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup);
	// 失活对象 对一群操作 -> 失活一群小兵
	void DisableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup);
	
	// 根据传入的对象名字  获取对象
	void GetSelfObject(TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup);
	// 根据传入的对象名称，获取这对象以外的，其他对象
	void GetOtherObject(TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup);
	// 根据类名，获取这个类中，除了传递参数的object的以外的其他对象
	void GetClassOtherObject(TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup);
	// 根据名字获取类对应的 所有对象
	void GetSelfClass(TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup);
	// 根据名字获取类以外的 所有对象
	void GetOtherClass(TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup);
	// 获得所有对象
	void GetAll(TArray<IDDOO*>& TargetObjectGroup);
	
	// 根据协议获取对象集合
	void GetAgreementObject(EAgreementType Agreement, TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup);
protected:
	// 框架对象数组 Ket: ObjectName , Value : *
	TMap<FName, IDDOO*> ObjectGroup;
	// 框架对象类集合, Key : ClassName , Value : TArray<IDDOO*>
	TMap<FName, TArray<IDDOO*>> ObjectClassGroup; // 根据类 类型存储，那就是TArray
	// 激活生命周期的对象集
	TArray<IDDOO*> ObjectActiveGroup;
	// 需要运行Tick函数的对象集合
	TArray<IDDOO*> ObjectTickGroup;

	
	//销毁 生命周期的对象集
	TArray<IDDOO*> ObjectDestroyGroup;
	//释放 周期函数对象集合, 只执行一次就全部释放
	TArray<IDDOO*> ObjectReleaseGroup;
	//预销毁对象组, 这些对象还处于激活生命周期, 但是在激活生命周期没运行完之前就已经设定为要进行销毁
	TArray<IDDOO*> PreObjectDestroyGroup;
};
