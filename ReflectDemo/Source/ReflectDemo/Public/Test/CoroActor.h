// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDObject/DDActor.h"
#include "CoroActor.generated.h"



struct CoroNode
{
	//激活状态
	bool IsActive;
	//剩余时间, 剩余帧
	float RemainTime;
	//条件委托
	FCoroCondition ConditionDele;
	//构造函数
	CoroNode() : IsActive(false) {}

	//延迟多少帧继续执行
	bool UpdateOperate(int32 SpaceTick)
	{
		if (!IsActive)
		{
			RemainTime = SpaceTick;
			IsActive = true;
			return true;
		}
		else
		{
			RemainTime -= 1;
			if (RemainTime > 0)
				return true;
			else
			{
				IsActive = false;
				return false;
			}
		}
	}

	//延迟秒条件 ,帧更新函数, 返回true, 就继续挂起, 返回false, 执行后续代码
	bool UpdateOperate(float DeltaTime, float SpaceTime)
	{
		if (!IsActive)
		{
			RemainTime = SpaceTime;
			IsActive = true;
			return true;
		}
		else
		{
			RemainTime -= DeltaTime;
			if (RemainTime > 0)
				return true;
			else
			{
				IsActive = false;
				return false;
			}
		}
	}

	//bool变量指针挂起, 变量为true则继续挂起, 为false则执行后续代码
	bool UpdateOperate(bool* Condition)
	{
		if (!IsActive)
		{
			IsActive = true;
			return true;
		}
		else
		{
			if (*Condition)
				return true;
			else
			{
				IsActive = false;
				return false;
			}
		}
	}

	//委托函数挂起, 函数返回true则继续挂起, 返回false则执行后续代码
	template<typename UserClass>
	bool UpdateOperate(UserClass* UserObj, typename FCoroCondition::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod)
	{
		if (!IsActive)
		{
			if (!ConditionDele.IsBound())
				ConditionDele.BindUObject(UserObj, InMethod);
			IsActive = true;
			return true;
		}
		else
		{
			if (ConditionDele.Execute())
				return true;
			else
			{
				IsActive = false;
				return false;
			}
		}
	}

	//lambuda表达式挂起
	bool UpdateOperate(TFunction<bool()> InFunction)
	{
		if (!IsActive)
		{
			IsActive = true;
			return true;
		}
		else
		{
			if (InFunction())
				return true;
			else
			{
				IsActive = false;
				return false;
			}
		}
	}

	//停止协程
	bool UpdateOperate()
	{
		IsActive = false;
		return true;
	}

};

struct CoroTask
{
	//多个协程节点
	TArray<CoroNode*> CoroStack;
	//构造函数
	CoroTask(int32 CoroCount)
	{
		for (int i = 0; i <= CoroCount; ++i)
			CoroStack.Push(new CoroNode());
	}
	//析构函数
	virtual ~CoroTask()
	{
		for (int i = 0; i < CoroStack.Num(); ++i)
			delete CoroStack[i];
	}
	//实例运行的帧函数
	virtual void Work(float DeltaTime) {}
	//是否完结状态
	bool IsFinish()
	{
		bool Flag = true;
		for (int i = 0; i < CoroStack.Num(); ++i)
		{
			if (CoroStack[i]->IsActive)
			{
				Flag = false;
				break;
			}
		}
		return Flag;
	}
};


/**
 * 
 */
UCLASS()
class REFLECTDEMO_API ACoroActor : public ADDActor
{
	GENERATED_BODY()

public:

	ACoroActor();

	virtual void DDEnable() override;

	virtual void DDTick(float DeltaSeconds) override;

protected:
	void CoroTestOne(float DeltaSeconds);
	CoroTask* CoroTestTwo();
	DDCoroTask* CoroTestThree();
	DDCoroTask* CoroFunc();


	
	DDCoroTask* CoroFixed();
	void StopCoro();
	

	void TempStartCoroutine(CoroTask* InTask);

	void DDStartCoroutine(DDCoroTask* InTask);

	void EchoCoroInfo();

	bool PauseFunPtr();

	bool PauseLambuda();

	void BKetEvent();

	
	void MultiKeyEvent();
protected:

	CoroNode TimeNode;

	TArray<CoroTask*> TaskList;

	TArray<DDCoroTask*> DDTaskList;

	FString CoroStr;

	
	bool IsCoroPause;

	int32 TimeCounter;
	
};
