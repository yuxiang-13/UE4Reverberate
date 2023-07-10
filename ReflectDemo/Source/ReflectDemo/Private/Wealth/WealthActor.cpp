// Fill out your copyright notice in the Description page of Project Settings.


#include "Wealth/WealthActor.h"

#include "Common/FWCommon.h"
#include "Engine/ObjectLibrary.h"
#include "Engine/StreamableManager.h"
#include "Wealth/WealthWidget.h"

// Sets default values
AWealthActor::AWealthActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	WorkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WorkMesh"));
	WorkMesh->SetupAttachment(RootComponent);

	
}

void AWealthActor::AssignWealthWidget(UWealthWidget* InWidget)
{
	wealthWidget = InWidget;

	UObjectUClassUBleprint();
}

// Called when the game starts or when spawned
void AWealthActor::BeginPlay()
{
	Super::BeginPlay();

	// WealthState();
	
	ObjectLibraryOperate();
	
	StreamableManagerOperate();

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
		// FWHelper::Debug(TextureData[i].ToSoftObjectPath().ToString(), 200.f);
		TexturePaths.AddUnique(TextureData[i].ToSoftObjectPath());
	}
}

void AWealthActor::StreamableManagerOperate()
{
	// 创建资源加载管理器
	WealthLoader = new FStreamableManager();
	// 执行异步加载，添加资源链接数组 和 加载完成回调
	WealthHandle = WealthLoader->RequestAsyncLoad(
	  TexturePaths,
		 FStreamableDelegate::CreateUObject(
			this,
			&AWealthActor::StreamableManagerLoadComplete
		 )
	);
}

// 加载完成后 动态修改 图片
void AWealthActor::StreamableManagerLoadComplete()
{
	// 获取资源数组
	TArray<UObject*> OutObjects;
	WealthHandle->GetLoadedAssets(OutObjects);

	for (int32 i = 0; i < OutObjects.Num(); ++i)
	{
		UTexture2D* WorkTexture = Cast<UTexture2D>(OutObjects[i]);
		if (WorkTexture)
		{
			TextureGroup.Add(WorkTexture);
		}
	}

	// 每隔0.5s切换图片
	FTimerDelegate UpdateTextureDele = FTimerDelegate::CreateUObject(this, &AWealthActor::UpdateTexture);
	GetWorld()->GetTimerManager().SetTimer(UpdateTextureHandle, UpdateTextureDele, 0.5f, true);
}

void AWealthActor::UpdateTexture()
{
	if (!wealthWidget) return;
	wealthWidget->AssignTexture(TextureGroup[TextureIndex]);
	TextureIndex = (TextureIndex + 1) >= TextureGroup.Num() ? 0 : (TextureIndex + 1);
}

void AWealthActor::UObjectUClassUBleprint()
{
	return;

	
	//第一步： 把 软引用 蓝图资源 加载到内存   否则第二步无效
	LoadObject<UObject>(NULL, *HandleObjectPath.GetAssetPathString());
	LoadObject<AActor>(NULL, *HandleActorPath.GetAssetPathString());
	LoadObject<UUserWidget>(NULL, *HandleWidgetPath.GetAssetPathString());

	//第二步，只从内存中获取Object
	UObject* HandleObject = HandleObjectPath.ResolveObject();
	UObject* HandleActor = HandleActorPath.ResolveObject();
	UObject* HandleWidget = HandleWidgetPath.ResolveObject();

	if (HandleObject) FWHelper::Debug("Get  HandObject");
	if (HandleActor) FWHelper::Debug("Get  HandleActor");
	if (HandleWidget) FWHelper::Debug("Get  HandleWidget");

	// 把内存中Object数据，进行实例化
	/*
	 注意：！！ 这种从 蓝图路径 获取Object，再Object->StaticClass() 去创建，直接崩溃
		UObject* InstObject = NewObject<UObject>(this, HandleObject->StaticClass());
		AActor* InstActor = GetWorld()->SpawnActor<AActor>(HandleActor->StaticClass(), GetActorTransform());
		UUserWidget* InstWidget = CreateWidget<UUserWidget>(GetWorld(), HandleWidget->StaticClass());
	 */

	UBlueprint* BlueObject = Cast<UBlueprint>(HandleObject);
	UBlueprint* BlueActor = Cast<UBlueprint>(HandleActor);
	UBlueprint* BlueWidget = Cast<UBlueprint>(HandleWidget);
	
	if (BlueObject) FWHelper::Debug("Get  BlueObject");
	if (BlueActor) FWHelper::Debug("Get  BlueActor");
	if (BlueWidget) FWHelper::Debug("Get  BlueWidget");

	/*
	 *
	 1 同步加载蓝图类为Class
	 FStreamableManager streamableManager;
	 FString strBPFileName = "/Game/ThirdPersonCPP/Blueprints/TestActor.TestActor_C";
	 UClass* pClass = streamableManager.LoadSynchronous<UClass>(FSoftObjectPath(strBPFileName));
	 if (pClass)
	 {
		 UE_LOG(LogTemp, Error, TEXT("UClass name is %s"), *pClass->GetName());
	 }

	 2 异步加载蓝图为UClass
	 void AMyProject7Character::BeginPlay()
	 {
		Super::BeginPlay();
		FStreamableManager streamableManager;
		FString strBPClassPath = "/Game/testActor.testActor_C";
		FStreamableDelegate streamableDelegate;
		FSoftClassPath SoftBPClassPathName = F Soft Class Path(strBPClassPath);
		streamableDelegate.BindUObject(this, &ThisClass::LoadFinish, SoftBPClassPathName);
		streamableManager.RequestAsyncLoad(SoftBPClassPathName, streamableDelegate);
	 }
	 
	 void AMyProject7Character::LoadFinish(FSoftClassPath SoftBPClassPathName)
	 {
			
			TSoftClassPtr<AActor> ActorClassPtr = TSoftClassPtr<AActor>(SoftBPClassPathName);
			UClass* pClass = ActorClassPtr.Get();
			if (pClass)
			{
				UE_LOG(LogTemp, Error, TEXT("UStaicMesh name is %s"), *pClass->GetName());
			}
	 }
	 
	// 1  同步加载UStaticMesh(非蓝图资源类)
	FStreamableManager streamableManager;
 
	FString strMeshFileName = "/Game/Geometry/Meshes/1M_Cube.1M_Cube";
	UStaticMesh* pStaticMesh = streamableManager.LoadSynchronous<UStaticMesh>(FSoftObjectPath(strMeshFileName));
	if (pStaticMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("Static Object name is %s"), *pStaticMesh->GetName());
	}

	
	// 2  异步加载UStaticMesh(非蓝图资源类)
	FString strMeshFileName = "/Game/Geometry/Meshes/1M_Cube.1M_Cube";
	FSoftObjectPath strMeshObjectFileName = FSoftObjectPath(strMeshFileName);
	
	FStreamableDelegate streamableDelegate;
	streamableDelegate.BindUObject(this, &ThisClass::LoadFinish, strMeshObjectFileName);
	
	FStreamableManager streamableManager;
	streamableManager.RequestAsyncLoad(strMeshObjectFileName, streamableDelegate);
	
	void xxxx::LoadFinish(FSoftObjectPath meshFilePath)
	{
		TSoftObjectPtr<UStaticMesh> MeshObjectPtr = TSoftObjectPtr<UStaticMesh>(meshFilePath);
		UStaticMesh* pStaticMesh = MeshObjectPtr.Get();
		if (pStaticMesh)
		{
			UE_LOG(LogTemp, Error, TEXT(%s), *pStaticMesh->GetName());
		}
	}
	*/
	
	
	
	// 下面出包后报错
	// UObject* InstObject = NewObject<UObject>(this, BlueObject->GeneratedClass );
	// AActor* InstActor = GetWorld()->SpawnActor<AActor>(BlueActor->GeneratedClass , GetActorTransform());
	// UUserWidget* InstWidget = CreateWidget<UUserWidget>(GetWorld(), (UClass*)BlueWidget->GeneratedClass );
	//
	// if (InstObject) FWHelper::Debug("Create  InstObject");
	// if (InstActor) FWHelper::Debug("Create  InstActor");
	// if (InstWidget) FWHelper::Debug("Create  InstWidget");
	//
	//
	// if (InstWidget) wealthWidget->AssignContent(InstWidget);

	
}


// Called every frame
void AWealthActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

