// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrame/FWPlayerController.h"

void AFWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 设置输入模式
	FInputModeGameAndUI InputMode;
	// 锁定鼠标
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// 设置视口捕捉鼠标时是否隐藏光标  Cursor 光标  英/ˈkɜːsə(r)/  : During 在期间 :   Capture 捕获 英/ˈkæptʃə(r)/ 
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	// 显示鼠标
	bShowMouseCursor = true;
}
