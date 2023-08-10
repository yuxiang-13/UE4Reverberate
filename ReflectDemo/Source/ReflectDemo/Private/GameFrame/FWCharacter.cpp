// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrame/FWCharacter.h"

#include "Common/FWCommon.h"

// Sets default values
AFWCharacter::AFWCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AFWCharacter::CAFuncOne(int32 Input, bool& Output)
{
}

void AFWCharacter::CAFuncFour(FString Info, int32 Count)
{
	//FWHelper::Debug(Info + FString(" - -- - ") + FString::FromInt(Count), 120.f);
}


#if WITH_EDITOR //editor
void AFWCharacter::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	// 1 看看所修改的变量PropertyChangedEvent.Property->GetName()
	// 2 GET_MEMBER_NAME_CHECKED 查找类下成员变量
	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(AFWCharacter, CAFive))
	{
		//FWHelper::Debug(FString(" - -- - ") + PropertyChangedEvent.Property->GetFName().ToString(), 120.f);
	}
}
#endif

void AFWCharacter::CAFuncThree_Implementation(int32 Input, bool& Output)
{
}

// Called when the game starts or when spawned
void AFWCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFWCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

