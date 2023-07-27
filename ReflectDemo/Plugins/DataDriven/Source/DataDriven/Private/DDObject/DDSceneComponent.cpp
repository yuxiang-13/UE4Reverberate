// Fill out your copyright notice in the Description page of Project Settings.


#include "DDObject/DDSceneComponent.h"

// Sets default values for this component's properties
UDDSceneComponent::UDDSceneComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAllowAnyoneToDestroyMe = true;
}

// Called when the game starts
void UDDSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	RegisterToModule(ModuleName, ObjectName, ClassName);
}

void UDDSceneComponent::DDRelease()
{
	IDDOO::DDRelease();
	DestroyComponent();
}

