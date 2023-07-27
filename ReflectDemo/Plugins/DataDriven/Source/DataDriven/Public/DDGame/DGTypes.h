// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DGTypes.generated.h"



#pragma region GamePart
/************************************************************************/
/* 开发的游戏的内容放这里              Begin            开发的游戏的内容放这里*/
/************************************************************************/



//模组约定, 如果要使用DataDriven本身的UI框架, 请一定要把HUD模组放在第二位, UIFrame框架管理器始终在HUD模组下
UENUM()
enum class ERCGameModule:uint8
{
	Center = 0, // 第一个，中央模组
	HUD,        // 下标为1 商量好了就是id = 1(UI模组, 约定放在第二位, UI框架会直接使用序号1去获取HUD模组, 如果不使用UI框架, 不需要强制在第二位)
	Player,     //玩家模组
}; 


/************************************************************************/
/* 开发的游戏的内容放这里               End             开发的游戏的内容放这里*/
/************************************************************************/
#pragma endregion


#pragma region LifeTimePart
	
// BaseObject 函数方法的 生命周期
UENUM()
enum class EBaseObjectLife: uint8
{
	None = 0, // 初始化状态
	Init,
	Loading,
	Register,
	Enable,
	Disable,
	UnRegister,
	UnLoading
};
// BaseObject 状态的 生命周期
UENUM()
enum class EBaseObjectState: uint8
{
	Active = 0, // 激活进程 的状态
	Stable, // 稳定进程 的状态
	Destroy // 销毁进程 的状态
};

#pragma endregion


// 反射系统代码块
#pragma region ReflectRart


// 对象查找协议
UENUM()
enum class EAgreementType:uint8
{
	SelfObject,		   // 通过传这个objectName,找到对应按个唯一的Object
	OtherObject,       //给传入的对象之外的对象通信
	ClassOtherObject,  //给传入的对象的相同类的 其他对象(不包括自己) 通信,调用这个方法要确保传过来的对象都是同一类的,如果不同类就多次通信
	
	SelfClass,         //给这个类的所有对象通信
	OtherClass,        //给这个类之外的类通信
	All                //给所有的对象通信
};

#pragma endregion





UCLASS()
class DATADRIVEN_API UDGTypes : public UObject
{
	GENERATED_BODY()
	
};
