// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/GameEngine.h"


#include "DDTypes.generated.h"

/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDDTypes : public UObject
{
	GENERATED_BODY()
	
};

// 1 #pragma once
// 大家应该都知道：指定该文件在编译源代码文件时仅由编译器包含（打开）一次。

// 2 #pragma region
// 用来展开、收缩代码区域。在一些函数比较多的文件中适用。

// 框架自定义Debug输出类
#pragma region LogDebug

// 用于绑定QT的Debug类
class DATADRIVEN_API DDRecord
{
private:
	// 自身单例
	static TSharedPtr<DDRecord> RecordInst;

	// 生成的字符串
	FString RecordInfo;
	// 显示时间
	float ShowTime;
	// 显示颜色
	FColor ShowColor;
public:
	// 打印模式： 0【Debug】 1【Log】 2【Warning】 3【Error】
	uint8 PatternID;
	
public:
	// 构造  析构
	inline  DDRecord() {}

	~DDRecord() {}

	static TSharedPtr<DDRecord> Get();

	// 设定参数
	inline void InitParam(float InTime, FColor InColor)
	{
		ShowTime = InTime;
		ShowColor = InColor;
	};
	// 执行打印
	inline void Output()
	{
		switch (PatternID)
		{
		case 0: // 打印到屏幕
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, ShowTime, ShowColor, RecordInfo);
				}
			}
			break;
		
		case 1: // 打印到Log
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), *RecordInfo);
			}
			break;
		case 2:
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *RecordInfo);
			}
			break;
		case 3:
			{
				UE_LOG(LogTemp, Error, TEXT("%s"), *RecordInfo);
			}
			break;
		}

		// 清理字符串
		RecordInfo.Empty();
	}

	// 1 重写操作符 <<  【 满足 = DDRecord::Get()类 出发自己类中 << 操作符】
	// 2 *** 【*this对指针解引用，因此使用*this返回的就是一个对象本身】
	// 3 << 函数参数重载，下面是FString参数函数
	inline DDRecord & operator<<(FString Info) { RecordInfo.Append(Info); return *this; }
	inline DDRecord & operator<<(FName Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord & operator<<(FText Info) { RecordInfo.Append(Info.ToString()); return *this; }
	
	inline DDRecord & operator<<(bool Info) { RecordInfo.Append( Info ? FString("True") : FString("False") ); return *this; }
	
	inline DDRecord & operator<<(const char* Info) { RecordInfo += Info; return *this; }
	inline DDRecord & operator<<(const char Info) { RecordInfo.AppendChar(Info); return *this; }

	inline DDRecord & operator<<(int32 Info) { RecordInfo.Append(FString::FromInt(Info)); return *this; }
	inline DDRecord & operator<<(float Info) { RecordInfo.Append(FString::SanitizeFloat(Info)); return *this; }
	inline DDRecord & operator<<(double Info) { RecordInfo.Append(FString::SanitizeFloat(Info)); return *this; }

	
	inline DDRecord & operator<<(FVector2D Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord & operator<<(FVector Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord & operator<<(FRotator Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord & operator<<(FQuat Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord & operator<<(FTransform Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord & operator<<(FMatrix Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord & operator<<(FColor Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord & operator<<(FLinearColor Info) { RecordInfo.Append(Info.ToString()); return *this; }

	// 输出字符串，匹配函数参数类型为自身  {使用例子  DDRecord::Get() << FString("ssss) << int << DDRecord::Get()}
	inline void operator<<(DDRecord& Record) { Record.Output(); };
};

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

// 调用结果，项目开发时，请确保每次都能第哦那个用成功
UENUM()
enum class ECallResult: uint8
{
	NoModule = 0, // 缺失模组
	LackObject, // 缺失对象
	NoFunction, // 缺失方法
	Succeed // 调用成功
};

// 通信参数结构体
struct DDParam
{
public:
	// 调用结果
	ECallResult CallResult;
	// 参数指针
	void *ParamPtr;
};

// 通信协议，Module模组方法
struct DDModuleAgreement
{
public:
	// 模组ID
	int32 ModuleIndex;
	// 方法名
	FName FunctionName;
};

// 通信协议，DDOO (对象) 方法
struct DDObjectAgreement
{
public:
	// 模组ID
	int32 ModuleIndex;
	// 协议类型
	EAgreementType AgreementType;
	// 对象组名
	TArray<FName> ObjectGroup;
	// 方法名
	FName FunctionName;
};

#pragma endregion




// 存储任意类型方法函数的结构体
#pragma region DDAnyFunc  // 英 /ˈriːdʒən/ 区域
 
// 存储任意方法结构体
struct DDAnyFun
{
	// 父类
	struct BaseFun
	{
	public:
		virtual ~BaseFun() {};
	};
	// 子类
	template<typename RetType, typename ... VarTypes>
	struct ChildValFun : public BaseFun
	{
	public:
		TFunction<RetType(VarTypes...)> TarFun;
	public:
		ChildValFun(const TFunction<RetType(VarTypes...)> InFun) : TarFun(InFun) {};
		RetType Execute(VarTypes... Params)
		{
			return TarFun(Params...);
		}
	};
	
public:
	BaseFun* FunPtr;
public:
	DDAnyFun() : FunPtr(NULL) {};
	template<typename RetType, typename ... VarTypes>
	DDAnyFun(const TFunction<RetType(VarTypes...)> InFun): FunPtr( new ChildValFun(InFun) ) {};
	
	~DDAnyFun() { delete FunPtr;  };

	// 执行
	template<typename RetType, typename ... VarTypes>
	RetType Execute(VarTypes ... Params)
	{
		// 强转成 子类 类指针,子类使用模板，那我们不动自动推导，手动指定模板类型
		ChildValFun<RetType, VarTypes...>* SubFunPtr = static_cast<ChildValFun<RetType, VarTypes...>*>(FunPtr);
		return SubFunPtr->Execute( Params ... );
	};
	// 获取 子类结构体中 保存的函数指针
	template<typename RetType, typename ... VarTypes>
	TFunction<RetType(VarTypes ...)>& GetFunc()
	{
		ChildValFun<RetType, VarTypes...>* SubFunPtr = static_cast<ChildValFun<RetType, VarTypes...>*>(FunPtr);
		return SubFunPtr.TarFun;
	}
	
};

#pragma endregion





#pragma region DDMsgNode

// 事件节点
struct DDMsgNode
{
	// 被调用的接口数量
	int32 CallCount;
	// 方法列表
	TMap<int32, DDAnyFun*> FunQuene;
	// 注册方法
	template<typename RetType, typename ... VarTypes>
	int32 RegisterFun(TFunction<RetType(VarTypes...)> InsFun);
	// 注销方法
	void UnRegisterFun(int32 FunID)
	{
		// 从列表移除对象
		DDAnyFun* DesPtr = *FunQuene.Find(FunID);
		FunQuene.Remove(FunID);
		delete DesPtr;
	}
	// 清空节点
	void ClearNode()
	{
		TArray<int32> KeysToDelete;
		for (auto& Pair : FunQuene)
		{
			KeysToDelete.Add(Pair.Key);
		}
		for (int32 i = KeysToDelete.Num() - 1; i >= 0; --i)
		{
			DDAnyFun* DesPtr = *FunQuene.Find(KeysToDelete[i]);
			delete DesPtr;
		}

	}
	// 执行方法
	template<typename RetType, typename ... VarTypes>
	RetType Execute(VarTypes ... Params);
	// 判断是否有绑定的函数
	bool IsBound() { return FunQuene.Num() > 0; };
	// 如果有绑定函数，就去执行
	template<typename RetType, typename ... VarTypes>
	bool ExecuteIfBound(VarTypes ... Params);
	// 构造
	DDMsgNode() : CallCount(0) {}
	
	
};

template <typename RetType, typename ... VarTypes>
int32 DDMsgNode::RegisterFun(TFunction<RetType(VarTypes...)> InsFun)
{
	// 获取方法序列里的所有下标
	TArray<int32> FunKeyQuene;
	FunQuene.GenerateKeyArray(FunKeyQuene);
	// 获取新下标
	int32 NewID;
	for (int32 i = FunKeyQuene.Num(); i>= 0; --i)
	{
		// 思考数组空时候，是否适用
		if (!FunKeyQuene.Contains(i))
		{
			NewID = i;
			break;
		}
	}
	// 将新方法添加到节点
	FunQuene.Add(NewID, new DDAnyFun(InsFun));
	return NewID;
}

template <typename RetType, typename ... VarTypes>
RetType DDMsgNode::Execute(VarTypes... Params)
{
	// 从第二个开始遍历
	TMap<int32, DDAnyFun*>::TIterator It(FunQuene);
	++It;
	for (; It; ++It)
	{
		It.Value()->Execute<RetType, VarTypes ...>(Params...);
	}
	// 返回第一个Map中的值
	TMap<int32, DDAnyFun*>::TIterator FirstIt(FunQuene);
	return FirstIt.Value()->Execute<RetType, VarTypes ...>(Params...);
}

template <typename RetType, typename ... VarTypes>
bool DDMsgNode::ExecuteIfBound(VarTypes... Params)
{
	if (!IsBound()) return false;

	for (TMap<int32, DDAnyFun*>::TIterator It(FunQuene); It; ++It)
	{
		It.Value()->Execute<RetType, VarTypes... >(Params...);
	}
	return true;
}

#pragma endregion





#pragma region DDCallHandle

struct DDMsgQuene;
// 调用句柄  （用于注册、注销）【名字 对应 节点】
template<typename RetType, typename ... Vartypes>
struct DDCallHandle
{
public:
	// 事件队列
	DDMsgQuene* MsgQuene;
	// 节点名, 调用名
	FName CallName;
	// 此一类方法是否是 激活状态,用于重写 = 等号操作符保存状态
	// 让所有 共用这个 共享指针的方法，不能二次注销，从而保证，只注销一次
	TSharedPtr<bool> IsActived;

public:
	// 构造函数
	DDCallHandle(){}
	DDCallHandle(DDMsgQuene* MQ, FName CN): MsgQuene(MQ), CallName(CN)
	{
		// 构造时，状态设置为激活状态
		IsActived = MakeShareable<bool>( new bool(true) );
	}
	// 重写操作符
	DDCallHandle<RetType, Vartypes ...>& operator= (const DDCallHandle<RetType, Vartypes ...>& Other)
	{
		if (this == &Other)
		{
			return *this;
		} else
		{
			MsgQuene = Other.MsgQuene;
			CallName = Other.CallName;
			IsActived = Other.IsActived;
			return *this;
		}
	}

	// 执行方法
	RetType Execute(Vartypes... Params);
	// 是否绑定
	bool IsBound();
	// 如果绑定就执行
	bool ExecuteIfBound(Vartypes... Params);
	// 注销调用接口
	void UnRegister();
};


template <typename RetType, typename ... Vartypes>
RetType DDCallHandle<RetType, Vartypes...>::Execute(Vartypes... Params)
{
	if (!IsBound() || ! *IsActived.Get() )
	{
		return NULL;
	} else
	{
		return MsgQuene->Execute<RetType, Vartypes...>(CallName, Params...);
	}
}

template <typename RetType, typename ... Vartypes>
bool DDCallHandle<RetType, Vartypes...>::IsBound()
{
	if ( ! *IsActived.Get() )
		return false;
	return MsgQuene->IsBound(CallName);
}

template <typename RetType, typename ... Vartypes>
bool DDCallHandle<RetType, Vartypes...>::ExecuteIfBound(Vartypes... Params)
{
	if (!IsBound() || ! *IsActived.Get())
		return false;
	MsgQuene->Execute<RetType, Vartypes...>(CallName, Params...);
	return true;
}

template <typename RetType, typename ... Vartypes>
void DDCallHandle<RetType, Vartypes...>::UnRegister()
{
	if (*IsActived.Get())
		MsgQuene->UnRegisterCallPort(CallName);

	*IsActived.Get() = false; // 让所有 共用这个 共享指针的方法，不能二次注销，从而保证，只注销一次
}

#pragma endregion



#pragma region DDMsgQuene

struct DDFunHandle;
// 事件队列
struct DDMsgQuene
{
	// 节点序列
	TMap<FName, DDMsgNode> MsgQuene;
	// 注册调用接口
	template<typename RetType, typename ... Vartypes>
	DDCallHandle<RetType, Vartypes...> RegisterCallPort(FName CallName);
	
	// 注册方法接口
	template<typename RetType, typename... VarTypes>
	DDFunHandle RegisterFunPort(FName CallName, TFunction<RetType(VarTypes...)> InsFun);
	// 注销 调用接口
	void UnRegisterCallPort(FName CallName)
	{
		if (!MsgQuene.Contains(CallName))
			return;
		// 获取时间节点
		DDMsgNode* MsgNode = MsgQuene.Find(CallName);
		// 让对应的节点调用计数器-1，如果计数器小于等于0，就移除调用接口
		MsgNode->CallCount--;
		if (MsgNode->CallCount <= 0)
		{
			MsgNode->ClearNode();
			MsgQuene.Remove(CallName);
		}
	}
	// 注销 方法接口
	void UnRegisterFunPort(FName CallName, int32 FunID)
	{
		if (!MsgQuene.Contains(CallName))
			return;
		MsgQuene.Find(CallName)->UnRegisterFun(FunID);
	}
	// 执行方法接口
	template<typename RetType, typename ... VarTypes>
	RetType Execute(FName CallName, VarTypes... Params);
	// 是否已经绑定方法
	bool IsBound(FName CallName) { return MsgQuene.Find(CallName)->IsBound(); };
};


template<typename RetType, typename ... Vartypes>
DDCallHandle<RetType, Vartypes...> DDMsgQuene::RegisterCallPort(FName CallName)
{
	// 如果已经存在对应CallName的调用接口，就把调用计数器+1
	if (MsgQuene.Contains(CallName))
	{
		MsgQuene.Find(CallName)->CallCount++;
	} else
	{
		// 创建新的事件节点 并且 添加到队列  ue4 ，struct结构体类A， 让 TMap存储  A() 和   TMap存储 new A() 的区别
		/*
		 * 在 UE4 中，当我们将结构体类 A 存储在 TMap 中时，它的实例不需要被手动删除，因为结构体的实例是在栈上分配和管理的，
		 * 并随着 TMap 的销毁而被自动销毁，这就是传值语义。
		 * 
		 * 而当我们将通过 new 操作符手动分配并生成的结构体类 A 的指针存储在 TMap 中时，它们是在堆上分配的。
		 * 在 TMap 销毁时，只会释放分配的空间，不会删除每个实例本身。如果不手动释放这些实例，将会导致内存泄漏和程序崩溃。
		 *
		 * 基本上，把结构体类 A 的实例以 new A() 的形式存储在 TMap 中是不必要的，应该直接存储 A 的实例。
		 */
		MsgQuene.Add(CallName, DDMsgNode());
		// 计数器+1
		MsgQuene.Find(CallName)->CallCount++;
	}
	// 返回调用句柄
	return DDCallHandle<RetType, Vartypes...>(this, CallName);
}

template<typename RetType, typename... VarTypes>
DDFunHandle DDMsgQuene::RegisterFunPort(FName CallName, TFunction<RetType(VarTypes...)> InsFun)
{
	//获取新的方法下标
	int32 FunID;
	//如果不存在CallName对应的节点
	if (!MsgQuene.Contains(CallName))
	{
		//创建新的事件节点并且添加到队列
		MsgQuene.Add(CallName, DDMsgNode());
	}
	//直接将新的方法注册到节点
	FunID = MsgQuene.Find(CallName)->RegisterFun(InsFun);
	//返回方法句柄
	DDFunHandle handle = DDFunHandle(this, CallName, FunID);;
	return handle;
}

template <typename RetType, typename ... VarTypes>
RetType DDMsgQuene::Execute(FName CallName, VarTypes... Params)
{
	return MsgQuene.Find(CallName)->Execute<RetType, VarTypes...>(Params ...);
}
#pragma endregion



#pragma region DDFunHandle

// 函数句柄
struct DDFunHandle
{
public:
	//消息队列
	DDMsgQuene* MsgQuene;
	//调用名字
	FName CallName;
	//方法ID
	int32 FunID;
	//是否有效
	TSharedPtr<bool> IsActived;
	//注销方法
public:
	DDFunHandle() {};
	DDFunHandle(DDMsgQuene* MQ, FName CN, int32 FI)
	{
		MsgQuene = MQ;
		CallName = CN;
		FunID = FI;
		// 设置状态为激活
		IsActived = MakeShareable<bool>(new bool(true));
	};
	// 重写=号操作符
	DDFunHandle& operator=(const DDFunHandle& Other)
	{
		if (this == &Other)
			return *this;
		MsgQuene = Other.MsgQuene;
		CallName = Other.CallName;
		FunID = Other.FunID;
		IsActived = Other.IsActived;
		return *this;
	};
	// 注销方法
	void UnRegister()
	{
		if (*IsActived.Get())
		{
			MsgQuene->UnRegisterFunPort(CallName, FunID);
		}
		// 设置为失活
		*IsActived.Get() = false;
	}
};

#pragma endregion




#pragma region Coroutine


// 判断条件委托
DECLARE_DELEGATE_RetVal(bool, FCoroCondition)

// 协程节点
struct DDCoroNode
{
	// 激活状态
	bool IsActive;
	// 剩余时间 或者 剩余帧
	float RemainTime;
	// 条件委托
	FCoroCondition ConditionDel;
	
	// 构造函数
	DDCoroNode() : IsActive(false) {};

	// 延迟多少帧 继续进行
	bool UpdateOperate(int32 SpaceTick)
	{
		if (!IsActive)
		{
			RemainTime = SpaceTick;
			IsActive = true;
			return true;
		} else
		{
			RemainTime -= 1;
			if (RemainTime > 0)
			{
				return true;
			} else
			{
				IsActive = false;
				return false;
			}
		}
	}
	
	// 延迟多少秒 帧更新函数 (参数-> 间隔，需要暂停的时间)  返回true就继续挂起，返回false就执行后续代码
	bool UpdateOperate(float DeltaTime, float SpaceTime)
	{
		if (!IsActive)
		{
			RemainTime = SpaceTime;
			IsActive = true;
			return true;
		} else
		{
			RemainTime -= DeltaTime;
			if (RemainTime > 0)
			{
				return true;
			} else
			{
				IsActive = false;
				return false;
			}
		}
	}

	// bool变量指针挂起，变量为true继续挂起，为false执行后续
	bool UpdateOperate(bool * Condition)
	{
		if (!IsActive)
		{
			IsActive = true;
			return true;
		} else
		{
			if (*Condition)
			{
				return true;
			} else
			{
				IsActive = false;
				return false; // 设置失活状态
			}
		}
	};

	// 委托函数 挂起，知道函数返回false才失活执行后续代码
	template<typename UserClass>
	bool UpdateOperate(UserClass* UserObj, typename FCoroCondition::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod)
	{
		if (!IsActive)
		{
			// 是否已经有绑定函数
			if (!ConditionDel.IsBound())
			{
				ConditionDel.BindUObject(UserObj, InMethod);
			}
			IsActive = true;
			return true;
		} else
		{
			if (ConditionDel.Execute())
			{
				return true; // 还是激活状态
			} else
			{
				IsActive = false;
				return false; // 设置失活状态
			}
		}
	}

	// Lambuda表达式挂起
	bool UpdateOperate(TFunction<bool()> InFunction)
	{
		if (!IsActive)
		{
			IsActive = true;
			return true;
		} else
		{
			if (InFunction())
			{
				return true;
			} else
			{
				IsActive = false;
				return false; // 设置失活状态
			}
		}
	};

	// 停止协程
	bool UpdateOperate()
	{
		IsActive = false; // 设置失活状态
		return true; // 返回true马上跳转到 函数结尾
	}
};

// 是不是结束协程节点
struct DDCoroTask
{
public:
	// 多个协程节点
	TArray<DDCoroNode*> CoroStack;
public:
	// 构造
	DDCoroTask(int32 CoroCount)
	{
		for (int i= 0; i <= CoroCount; ++i)
		{
			CoroStack.Push(new DDCoroNode());
		}
	}
	// 析构
	virtual ~DDCoroTask()
	{
		for (int i= CoroStack.Num() - 1; i >= 0; --i)
		{
			delete CoroStack[i];
		}
	}
	// 实际运行的帧函数
	virtual void Work(float DeltaTime) {};
	// 是否完结状态
	bool IsFinish()
	{
		bool Flag = true;
		for (int i= 0; i < CoroStack.Num(); ++i)
		{
			// 循环所有 协程节点，只要有一个再激活状态（还在挂起），那就还不能运行
			if (CoroStack[i]->IsActive)
			{
				Flag = false;
				break;
			}
		}

		return Flag;
	}
};

#pragma endregion