// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/CoroActor.h"

ACoroActor::ACoroActor()
{
	IsAllowTickEvent = true;
}

void ACoroActor::DDEnable()
{
	Super::DDEnable();

	CoroStr = FString("CoroStr");

	IsCoroPause = true;

	//TempStartCoroutine(CoroTestTwo());

	// DDH::Debug(10) << "StartCoroutine --> " << StartCoroutine("CoroTest", CoroTestThree()) << DDH::Endl();
	
	// InvokeRepeat("EchoInfo", 3.f, 2.f, this, &ACoroActor::EchoCoroInfo);
	
	// StartCoroutine("CoroFixed", CoroFixed());

	
	// DDBindInput(this, &ACoroActor::BKetEvent, EKeys::B, IE_Pressed);

	DDBindInput(this, &ACoroActor::MultiKeyEvent, EKeys::A, EKeys::S, EKeys::D);
}



void ACoroActor::DDTick(float DeltaSeconds)
{
	Super::DDTick(DeltaSeconds);

	//CoroTestOne(DeltaSeconds);



	//协程帧循环逻辑
	/*TArray<CoroTask*> TempTask;

	for (int i = 0; i < TaskList.Num(); ++i)
	{
		TaskList[i]->Work(DeltaSeconds);
		if (TaskList[i]->IsFinish())
			TempTask.Push(TaskList[i]);
	}
	for (int i = 0; i < TempTask.Num(); ++i)
	{
		TaskList.Remove(TempTask[i]);
		delete TempTask[i];
	}*/


	TArray<DDCoroTask*> TempTask;

	for (int i = 0; i < DDTaskList.Num(); ++i)
	{
		DDTaskList[i]->Work(DeltaSeconds);
		if (DDTaskList[i]->IsFinish())
			TempTask.Push(DDTaskList[i]);
	}
	for (int i = 0; i < TempTask.Num(); ++i)
	{
		DDTaskList.Remove(TempTask[i]);
		delete TempTask[i];
	}


	TimeCounter++;
	if (TimeCounter == 500)
		IsCoroPause = false;

}

void ACoroActor::CoroTestOne(float DeltaSeconds)
{
	//等待3秒
	if (TimeNode.UpdateOperate(DeltaSeconds, 3.f))
		goto LABEL_END;

	//运行逻辑代码
	DDH::Debug(10) << "CoroTestOne" << DDH::Endl();

LABEL_END:
	;
}

CoroTask* ACoroActor::CoroTestTwo()
{
	struct ValueTask : public CoroTask
	{
		ACoroActor* D;
		ValueTask(ACoroActor* Data, int32 CoroCount) : CoroTask(CoroCount) { D = Data; }

		//所有需要保存状态的变量都需要定义成类变量
		//所有循环判断的变量都需要定义成类变量

		int32 i;

		int32 j;

		virtual void Work(float DeltaTime) override
		{
			goto LABEL_PICKER;
		LABEL_START:


			DDH::Debug(10) << 0 << DDH::Endl();


		LABEL_0:
			if (CoroStack[0]->UpdateOperate(&(D->IsCoroPause)))
				goto LABEL_END;

			DDH::Debug(10) << 1 << DDH::Endl();


		LABEL_1:
			if (CoroStack[1]->UpdateOperate())
				goto LABEL_END;

			DDH::Debug(10) << 2 << DDH::Endl();


		LABEL_2:
			if (CoroStack[2]->UpdateOperate([this]() { return D->PauseLambuda(); }))
				goto LABEL_END;


			DDH::Debug(10) << 3 << DDH::Endl();



			goto LABEL_END;
		LABEL_PICKER:
			if (CoroStack[0]->IsActive) goto LABEL_0;
			if (CoroStack[1]->IsActive) goto LABEL_1;
			if (CoroStack[2]->IsActive) goto LABEL_2;
			goto LABEL_START;
		LABEL_END:
			;
		}
	};
	return new ValueTask(this, 2);
}

DDCoroTask* ACoroActor::CoroTestThree()
{
	//协程参数区
	DDCORO_PARAM(ACoroActor);

	int32 i;
	int32 j;

	//协程方法主体开始
#include DDCORO_BEGIN()




	DDH::Debug(10) << 0 << DDH::Endl();

#include DDYIELD_READY()
	DDYIELD_RETURN_BOOL(&(D->IsCoroPause));

	DDH::Debug(10) << 1 << DDH::Endl();

#include DDYIELD_READY()
	DDYIELD_RETURN_LAMB(D->PauseLambuda(););

	DDH::Debug(10) << 2 << DDH::Endl();


#include DDYIELD_READY()
	DDYIELD_RETURN_FUNC(D, &ACoroActor::PauseFunPtr);

	DDH::Debug(10) << 3 << DDH::Endl();


	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 5; j++)
		{
#include DDYIELD_READY()
			DDYIELD_RETURN_TICK(20);

			DDH::Debug(10) << i << j << DDH::Endl();

			if (i * 10 + j > 30)
			{
#include DDYIELD_READY()
				DDYIELD_RETURN_STOP();
			}

		}
	}


#include DDYIELD_READY()
	DDYIELD_RETURN_SECOND(5.f);

	DDH::Debug(10) << 4 << DDH::Endl();





	//协程方法主体结束
#include DDCORO_END()
}

DDCoroTask* ACoroActor::CoroFunc()
{
	//协程参数区
	DDCORO_PARAM(ACoroActor);

	//协程方法变量


	//协程方法主体开始
#include DDCORO_BEGIN()


	//协程方法主体逻辑代码
#include DDYIELD_READY()
	DDYIELD_RETURN_SECOND(5.f);


	//协程方法主体结束
#include DDCORO_END()
}

DDCoroTask* ACoroActor::CoroFixed()
{
	DDCORO_PARAM(ACoroActor);
#include DDCORO_BEGIN()

#include DDYIELD_READY()
	DDYIELD_RETURN_SECOND(5.f);
	DDH::Debug(10) << "StopCoro 1111" << DDH::Endl();
	
	D->StopCoro();

#include DDYIELD_READY()
	DDYIELD_RETURN_SECOND(3.f);
	DDH::Debug(10) << "StopCoro 2222" << DDH::Endl();


#include DDCORO_END()
}

void ACoroActor::StopCoro()
{
	StopCoroutine("CoroFixed");
}

	void ACoroActor::TempStartCoroutine(CoroTask* InTask)
{
	TaskList.Push(InTask);
}

void ACoroActor::DDStartCoroutine(DDCoroTask* InTask)
{
	DDTaskList.Push(InTask);
}

void ACoroActor::EchoCoroInfo()
{
	DDH::Debug(10) << "EchoCoroInfo" << DDH::Endl();
}

bool ACoroActor::PauseFunPtr()
{
	static int32 FunPtrTime = 0;
	FunPtrTime++;
	if (FunPtrTime == 300)
		return false;
	return true;
}

bool ACoroActor::PauseLambuda()
{
	static int32 LambudaTime = 0;
	LambudaTime++;
	if (LambudaTime == 600)
		return false;
	return true;
}


void ACoroActor::BKetEvent()
{
	DDH::Debug(10) << "BKetEvent" << DDH::Endl();
}

void ACoroActor::MultiKeyEvent()
{
	DDH::Debug(10) << "MultiKeyEvent" << DDH::Endl();
}


#if 0
DDCoroTask* ACoroActor::CoroTestThree()
{
// 	//协程参数区块
// #pragma region DDCORO_PARAM
// 	struct DGCoroTask : public DDCoroTask
// 	{
// 		ACoroActor* D;
// 		DGCoroTask(ACoroActor* Data, int32 CoroCount) : DDCoroTask(CoroCount) { D = Data; }
// #pragma endregion
//
//
// 		//用来定义类变量, 需要保存状态字变量
//
//
// #define DDYIELD_COUNT -1
// //Work方法开头
// #pragma region DDCORO_WORK_START
// 		virtual void Work(float DeltaTime) override
// 		{
// 			goto DDCORO_LABEL_PICKER;
// 		DDCORO_LABEL_START:
// #pragma endregion
//
//
// 			//协程方法逻辑
// #pragma region CoroFunCode
//
// #if DDYIELD_COUNT == -1
// #define DDYIELD_COUNT 0
// 			DDCORO_LABEL_0 :
// #elif DDYIELD_COUNT == 0
// #define DDYIELD_COUNT 1
// 			DDCORO_LABEL_1 :
// #endif
// 				if (CoroStack[DDYIELD_COUNT]->UpdateOperate(&(D->IsCoroPause)))
// 					goto DDCORO_LABEL_END;
//
//
// #if DDYIELD_COUNT == -1
// #define DDYIELD_COUNT 0
// 						   DDCORO_LABEL_0 :
// #elif DDYIELD_COUNT == 0
// #define DDYIELD_COUNT 1
// 						   DDCORO_LABEL_1 :
// #endif
// 							   if (CoroStack[DDYIELD_COUNT]->UpdateOperate(10))
// 								   goto DDCORO_LABEL_END;
//
// #pragma endregion
//
//
// 										  //Work方法中间
// #pragma region DDCORO_WORK_MIDDLE
// 										  goto DDCORO_LABEL_END;
// 									  DDCORO_LABEL_PICKER:
// #pragma endregion
//
//
// 										  //协程条件跳转代码
// #pragma region CoroPicker
// #if DDYIELD_COUNT == 0
// 										  if (CoroStack[0]->IsActive) goto DDCORO_LABEL_0;
// #elif DDYIELD_COUNT == 1
// 										  if (CoroStack[0]->IsActive) goto DDCORO_LABEL_0;
// 										  if (CoroStack[1]->IsActive) goto DDCORO_LABEL_1;
// #endif
//
// #pragma endregion
//
//
// 										  //Work方法结尾
// #pragma region DDCORO_WORK_END
// 										  goto DDCORO_LABEL_START;
// 									  DDCORO_LABEL_END:
// 										  ;
// 		}
// 	};
// 	return new DGCoroTask(this, DDYIELD_COUNT);
// #pragma endregion
//

}
#endif