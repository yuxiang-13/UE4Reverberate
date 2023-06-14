// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FWDataSingleton.generated.h"

class UTexture2D;

/**
 *  蓝图类 可继承，可访问
 */
UCLASS(Blueprintable, BlueprintType)
class REFLECTDEMO_API UFWDataSingleton : public UObject
{
	GENERATED_BODY()
public:
	UFWDataSingleton();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrameWork")
	TArray<UClass*> SourceBlueprints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrameWork")
	UTexture2D* SourceT2D;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrameWork")
	FVector SourceVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrameWork")
	FString SourceFilePath;
	
};
