// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/FWAffectWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Common/FWCommon.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

UFWAffectWidget::UFWAffectWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

bool UFWAffectWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	// 第一种获取方式，强转子集
	RootPanel = Cast<UOverlay>(GetRootWidget());
	if (RootPanel)
	{
		BGImage = Cast<UImage>(RootPanel->GetChildAt(0));
	}
	
	// 第二种获取方式， GetWidgetFromName
	UButton* ButtonTwo = (UButton*) GetWidgetFromName(TEXT("ButtonTwo"));

	// 绑定按钮事件 方式一： __Internal_AddDunamic
	ButtonOne->OnClicked.__Internal_AddDynamic(this, &UFWAffectWidget::ButtonOneEvent, FName("ButtonOneEvent"));
	
	// 绑定按钮事件 方式二： 用委托
	FScriptDelegate ButtonTwoDel;
	ButtonTwoDel.BindUFunction(this, "ButtonTwoEvent");
	ButtonTwo->OnReleased.Add(ButtonTwoDel);

	
	return true;
}

void UFWAffectWidget::ButtonOneEvent()
{
	FWHelper::Debug("ButtonOneEvent 111", 10.f);
	// LoadObject 同步
	UTexture2D* Tartex = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Resource/UI/Texture/MenuTex/book.book'"));
	BGImage->SetBrushFromTexture(Tartex);
	
}

void UFWAffectWidget::ButtonTwoEvent()
{
	FWHelper::Debug("ButtonTwoEvent 222", 10.f);
	// 如果动态Image已经存在，直接返回
	if (DynImage) return;

	
	// 创建Widget的方法二，通过WidgetTree创建组件
	// WidgetTree （每一个userwidget都有一个WidgetTree变量）
	DynImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
	// 添加新image控件到Panel
	UOverlaySlot* DynImageSlot = RootPanel->AddChildToOverlay(DynImage);
	// 设置变换 .... 啥啥的
	// DynImageSlot->SetPadding()
	
}

// 销毁 按钮2 创建的新插槽
void UFWAffectWidget::ButtonThreeEvent()
{
	FWHelper::Debug("ButtonThreeEvent 333", 10.f);
	
	// 如果动态Image已经存在，直接返回
	if (!DynImage) return;

	// 移除父级
	RootPanel->RemoveChild(DynImage);
	// 或者 DynImage->RemoveFromParent();
 
	// 移除成功，要释放资源 Conditional 有条件的 英/kənˈdɪʃənl/
	DynImage->ConditionalBeginDestroy(); // UObejct 自己的垃圾回收
	DynImage = NULL;
}