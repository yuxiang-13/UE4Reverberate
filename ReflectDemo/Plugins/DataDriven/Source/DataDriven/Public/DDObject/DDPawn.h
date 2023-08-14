// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DDOO.h"
#include "DDPawn.generated.h"

UCLASS()
class DATADRIVEN_API ADDPawn : public APawn, public IDDOO
{
	GENERATED_BODY()


public:
	// Sets default values for this pawn's properties
	ADDPawn();

	//重写释放函数
	virtual void DDRelease() override;

public:

	//模组名字,如果为空,说明要手动指定,不为空就是自动指定
	UPROPERTY(EditAnywhere, Category = "DataDriven")
	FName ModuleName;
	//对象名字,如果为空,说明要手动指定,不为空就是自动指定
	UPROPERTY(EditAnywhere, Category = "DataDriven")
	FName ObjectName;
	//类名字,如果为空,说明要手动指定,不为空就是自动指定
	UPROPERTY(EditAnywhere, Category = "DataDriven")
	FName ClassName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
