// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DDOO.h"
#include "DDGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DATADRIVEN_API ADDGameModeBase : public AGameModeBase, public IDDOO
{
	GENERATED_BODY()
	
public:

	ADDGameModeBase();


public:

	//ģ������,���Ϊ��,˵��Ҫ�ֶ�ָ��,��Ϊ�վ����Զ�ָ��
	UPROPERTY(EditAnywhere, Category = "DataDriven")
		FName ModuleName;
	//��������,���Ϊ��,˵��Ҫ�ֶ�ָ��,��Ϊ�վ����Զ�ָ��
	UPROPERTY(EditAnywhere, Category = "DataDriven")
		FName ObjectName;
	//������,���Ϊ��,˵��Ҫ�ֶ�ָ��,��Ϊ�վ����Զ�ָ��
	UPROPERTY(EditAnywhere, Category = "DataDriven")
		FName ClassName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
