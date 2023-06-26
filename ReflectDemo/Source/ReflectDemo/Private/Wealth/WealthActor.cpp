// Fill out your copyright notice in the Description page of Project Settings.


#include "Wealth/WealthActor.h"

#include "Common/FWCommon.h"
#include "Engine/ObjectLibrary.h"

// Sets default values
AWealthActor::AWealthActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	WorkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WorkMesh"));
	WorkMesh->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void AWealthActor::BeginPlay()
{
	Super::BeginPlay();

	// WealthState();
	
	ObjectLibraryOperate();

	// FTimerDelegate UpdateMesgDele = FTimerDelegate::CreateUObject(this, &AWealthActor::UpdateMesh);
	// GetWorld()->GetTimerManager().SetTimer(UpdateMeshHandle, UpdateMesgDele, 1.f, true);
}

void AWealthActor::WealthState()
{
	// LoadObject(); 这个会把硬盘中资源加载到内存
	// FindObject(); 这个只能在内存中查找资源

	UStaticMesh* BlockMesh = FindObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Resource/SCTanks/Meshes/SK_SCT_Block.SK_SCT_Block'"));
	if (!BlockMesh) FWHelper::Debug("FindObject BlockMesh Failed");

	FWHelper::Debug("Loading Object BlockMesh");
	LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Resource/SCTanks/Meshes/SK_SCT_Block.SK_SCT_Block'"));
	
	BlockMesh = FindObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Resource/SCTanks/Meshes/SK_SCT_Block.SK_SCT_Block'"));
	if (!BlockMesh) FWHelper::Debug("FindObject BlockMesh Failed");
	WorkMesh->SetStaticMesh(BlockMesh);

	
}

void AWealthActor::UpdateMesh()
{
	if (WealthData && WealthData->WealthGroup.Num() > 0)
	{
		UStaticMesh* FactMesh = LoadObject<UStaticMesh>(NULL, *(WealthData->WealthGroup[MeshIndex].WealthPath.ToString()));
		WorkMesh->SetStaticMesh(FactMesh);
		MeshIndex = (MeshIndex + 1) >= WealthData->WealthGroup.Num() ? 0 : (MeshIndex + 1);
	}
}

void AWealthActor::ObjectLibraryOperate()
{
	if (!ObjectLibrary)
	{
		ObjectLibrary = UObjectLibrary::CreateLibrary(UObject::StaticClass(), false, false);
		// 防止被垃圾回收销毁
		ObjectLibrary->AddToRoot();
	}

	// 搜索路径内所有Texture资源路径
	ObjectLibrary->LoadAssetDataFromPath(TEXT("/Game/Resource/UI/Texture/MenuTex"));

	// 获取文件夹内所有资源AssetData，此时还没加载到内存
	TArray<FAssetData> TextureData;
	ObjectLibrary->GetAssetDataList(TextureData);

	// 获取所有资源路径，保存,一会异步加载会用到
	for (int32 i = 0; i < TextureData.Num(); i++)
	{
		FWHelper::Debug(TextureData[i].ToSoftObjectPath().ToString(), 200.f);
		TexturePaths.AddUnique(TextureData[i].ToSoftObjectPath());
	}
}

// Called every frame
void AWealthActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

