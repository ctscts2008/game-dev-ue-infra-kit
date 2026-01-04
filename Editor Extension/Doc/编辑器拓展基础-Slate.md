> 《调教UE5》系列记录的是笔者在开发UE引擎中总结的些许经验。文中所有观点和结论仅代表个人见解，作为自学笔记和日后反思之参考，亦可能存在谬误或过时之处。如有错漏和不当，恳请多加指正。


---


[Helo：《调教UE5：编辑器拓展指南》开篇及索引](https://zhuanlan.zhihu.com/p/604684703)


---


# 目录

> **不可忽略的事前准备：新建 CustomSlate 模块 ~ **♡ 
> **浅尝辄逝的心动体验：Slate 应用 ~** ♡
> **ㅤSpawn Window**
> **ㅤSpawn Tab**
> **ㅤ可布局的 Tabs ：TabManager**
> **ㅤFTabManager 简要归纳**
> **ㅤ在 Slate 中使用 Widget**
> **深入发掘的无限魅力：Slate 基础 ~ **♡ 
> **ㅤSlate 概述**
> **ㅤSlate 的创建和入口**
> **ㅤ链式编程**
> **ㅤ自定义 Slate 控件**
> **ㅤㅤ**ㅤ**Slate 控件构成**
> **ㅤㅤ**ㅤ**FArguments 想要装起来**
> **ㅤㅤㅤㅤㅤ“ . ”是怎么来的：FArguments&**
> **ㅤㅤㅤㅤㅤ“+” 是怎么来的：SLATE_SLOT_ARGUMENT**
> **ㅤㅤㅤㅤㅤ“[ ]”是怎么来的：SLATE_DEFAULT_SLOT & SLATE_NAMED_SLOT**
> **ㅤㅤㅤㅤㅤSLATE_ARGUMENT 想要做自己**
> **ㅤㅤㅤㅤㅤSLATE_ATTRIBUTE 想要绑起来**
> **ㅤㅤㅤㅤㅤSLATE_EVENT 想要发出去**
> **ㅤㅤㅤConstruct 想要拿进来**
> **ㅤㅤㅤTSlateAttribute 与 TSlateManagedAttribute**
> **ㅤㅤㅤOnPaint 快要忙死了**
> **ㅤㅤㅤComputeDesiredSize 觉得不够大**





---


$$
\large \textbf{世上没有什么事情比必然与偶然更难懂了，} \\ \large \textbf{就像要懂得木头人的爱恋之情一样困难。} \\
$$


---


本章指南是 [《调教UE5：编辑器拓展指南》Widget 相关基础](https://zhuanlan.zhihu.com/p/607360162) 的姊妹篇，如果你已经阅读过该章内容，可直接前往本章“浅尝辄逝的心动体验：Slate 应用 ~ ♡”部分，进行后续阅读。


---


# 不可忽略的事前准备：CustomSlate 模块 ~ ♡

在正式开始之前，我们来进行一些不可或缺的预前准备。

目前，本指南基于 **UE5.0.3** 引擎版本进行编写，读者需前往下载对应的引擎版本，以免在尝试过程中因引擎版本差异造成不必要的麻烦。

为了拥有相对易读的内容，我们来为本章内容准备一个独立的模块，并在编辑器 ToolBar 栏上新建一个按钮。




## 创建 CustomSlate 模块

① 在项目“Source”文件夹下新建如下结构。

- CustomSlate 文件夹
- CustomSlate.h
- CustomSlate.cpp
- CustomSlate.Build.cs

![](https://picx.zhimg.com/v2-9ea87902de20200b9049dd1a40fac1c3_r.jpg)




② 打开 CustomSlate.h，声明 FCustomSlateModule 模块。

```cpp
// CustomSlate.h

#pragma once

#include "Modules/ModuleInterface.h"

class FCustomSlateModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual ~FCustomSlateModule() {}
};
```




③ 打开 CustomSlate.cpp，实现 FCustomSlateModule 模块，将其命名为“CustomSlate”。

```cpp
// CustomSlate.cpp

#pragma once

#include "CustomSlate.h"

IMPLEMENT_MODULE(FCustomSlateModule, CustomSlate)

void FCustomSlateModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FCustomSlateModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
```




④ 打开 CustomSlate.Build.cs，设置模块规则

```csharp
// CustomSlate.Build.cs

using UnrealBuildTool;

public class CustomSlate : ModuleRules
{
	public CustomSlate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
```




⑤ 来到项目 Editor.Target.cs 文件，暂时将“CustomSlate”模块添加到其中以使 IDE 能够正常识别模块。

```csharp
// ExtendEditorEditor.Target.cs

using UnrealBuildTool;
using System.Collections.Generic;

public class ExtendEditorEditorTarget : TargetRules
{
	public ExtendEditorEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[]
		{
			...

			"CustomSlate",
		} );
	}
}
```




⑥ 来到项目 .uproject 文件，为模块设置加载规则。

```csharp
// ExtendEditor.uproject

{
	"FileVersion": 3,
	"EngineAssociation": "5.0",
	"Category": "",
	"Description": "",
	"Modules": [
		...

		{
			"Name": "CustomSlate",
			"Type": "Editor",
			"LoadingPhase": "Default"
		}
	],
	...
}
```







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## 在 ToolBar 栏上创建新按钮

① 来到 CustomSlate.h 中为创建操作添加新的入口函数。

```cpp
// CustomSlate.h

#pragma once

#include "Modules/ModuleInterface.h"

class FCustomSlateModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual ~FCustomSlateModule() {}

private:

	/** 创建新Button */
	void CreateButtonByStyle();
	void ButtonAction();
};
```




② 来到 CustomSlate.cpp 中实现函数。有关 ExtendEditorStyle 模块的创建方法，可以参考 [《调教UE5：编辑器拓展指南》编辑器拓展基础](https://zhuanlan.zhihu.com/p/605181368) 中“FSlateStyleSet”小节。

如果你没有从 [《调教UE5：编辑器拓展指南》编辑器拓展基础](https://zhuanlan.zhihu.com/p/605181368) 里的基础步骤开始本指南，FSlateIcon() 可以省略括号中的内容，以缺省设置。你将会获得一个没有 Icon 的按钮。或者你可以使用系统自带的图标样式。

```cpp
// CustomSlate.cpp

#pragma once

#include "CustomSlate.h"

#include "ToolMenus.h"
#include "ExtendEditorStyle.h"

void FCustomSlateModule::StartupModule()
{
	CreateButtonByStyle();
}

void FCustomSlateModule::CreateButtonByStyle()
{
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.User");

	FToolMenuSection& Section = Menu->FindOrAddSection(NAME_None);
	Section.AddEntry(FToolMenuEntry::InitToolBarButton(
		"Custom Slate Button",
		FToolUIActionChoice(FExecuteAction::CreateRaw(this, &FCustomSlateModule::ButtonAction)),
		FText::FromString("Custom Slate Button"),
		FText::FromString("This is a custom slate button."),
		FSlateIcon(FExtendEditorStyle::GetStyleSetName(), "AliceTool.Slate"))
		);
}

void FCustomSlateModule::ButtonAction()
{
	// Do something ...
}
```




③ 为了使用“AliceTool.Slate”作为新的自定义样式图标，我们需要在 FExtendEditorStyle 中去注册对应的图标。

来到 ExtendEditorStyle.cpp 中，在 FExtendEditorStyle::CreateSlateStyleSet() 中添加如下内容。

```cpp
// ExtendEditorStyle.cpp

TSharedRef<FSlateStyleSet> FExtendEditorStyle::CreateSlateStyleSet()
{
	...

	{
		const FVector2D IconeSize(16.f, 16.f);
		FSlateImageBrush* SlateImageBrush = 
			new FSlateImageBrush(RootPath + TEXT("AliceIcon_Slate.png"), IconeSize);
		SlateStyleSet->Set("AliceTool.Slate", SlateImageBrush);
	}
	
	return SlateStyleSet;
}
```

然后来到项目文件夹下的 Resource 文件夹中，将同名的图标资产添加进来。

![](https://pic4.zhimg.com/v2-c63638251bb548e8ba3608d1427477f3_r.jpg)




④ 来到 CustomSlate.Build.cs 中，添加模块依赖项。

```csharp
// CustomSlate.Build.cs

using UnrealBuildTool;

public class CustomSlate : ModuleRules
{
	public CustomSlate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{ 
			...
			"ExtendEditorStyle",
			"SlateCore",
			"ToolMenus",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
```

重新编译并重启编辑器，我们就应该可以看到按钮被正确添加了。

![](https://pica.zhimg.com/v2-e6bca472a25f7afb97dff4b1d025a856_r.jpg)







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 


 

# 浅尝辄逝的心动体验：Slate 应用 ~ ♡




$$
怀念总是在带来愉快的心情后，又带来寂寞。 \\
$$







## Spawn Window

接下来我们来探索如何生成新的窗口。

来到 CustomSlate.h 中，添加生成窗口的入口函数。

```cpp
// CustomSlate.h

#pragma once

#include "Modules/ModuleInterface.h"

class FCustomSlateModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual ~FCustomSlateModule() {}

private:

	/** 创建新Button */
	void CreateButtonByStyle();
	void ButtonAction();

	/** 创建窗口 */
	void SpawnWindow();
};
```




接着来到 CustomSlate.cpp 中，将入口函数添加到 ButtonAction()，并实现。

```cpp
// CustomSlate.cpp

...

#include "Widgets/Layout/SConstraintCanvas.h"

...

void FCustomSlateModule::ButtonAction()
{
	SpawnWindow();
}

void FCustomSlateModule::SpawnWindow()
{
	// 创建新窗口
	TSharedRef<SWindow> Window = SNew(SWindow)
	.Title(FText::FromString("Alice Window"))
	.SizingRule(ESizingRule::UserSized)
	.ClientSize(FVector2D(1000.f, 700.f))
	.AutoCenter(EAutoCenter::PreferredWorkArea)
	.ScreenPosition(FVector2D(0, 0))
	.IsEnabled(true)
	.SupportsMinimize(true)
	.SupportsMaximize(true);
	
	// 设置窗口内容
	Window->SetContent
	(
		SNew(SConstraintCanvas)
		+SConstraintCanvas::Slot()
		.Alignment(0.5f)
		.Anchors(0.5f)
		.Offset(FMargin(0.0f, 0.0f, 300.f, 100.f))
		.AutoSize(true)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Hello, Alice Window !"))
		]
	);

	// 显示新窗口
	FSlateApplication::Get().AddWindow(Window);
}
```




重新编译并重启编辑器，点击添加的按钮，就可以看到新窗口被打开了。

![](https://pic3.zhimg.com/v2-6d42a37fed53ca71c42065a36efa3772_b.gif)







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## Spawn Tab

接下来我们来探索如何生成新的 Tab。对于编辑器拓展来说，Tab 比 Window 更常用，因为它具有可以停靠和支持布局的特性。

来到 CustomSlate.h 中，添加生成 Tab 的入口函数。

```cpp
// CustomSlate.h

#pragma once

#include "Modules/ModuleInterface.h"

class FCustomSlateModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual ~FCustomSlateModule() {}

private:

	/** 创建新Button */
	void CreateButtonByStyle();
	void ButtonAction();

	/** 创建Tab */
	void SpawnTab();
	TSharedRef<SDockTab> OnSpawnTab(const FSpawnTabArgs& Args);
};
```




接着来到 CustomSlate.cpp 中，将入口函数添加到 ButtonAction()，并实现。

```cpp
// CustomSlate.cpp

...

#include "Widgets/Layout/SConstraintCanvas.h"

...

void FCustomSlateModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();

	// 在模块注销时注销
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FName("AliceTab"));
}

void FCustomSlateModule::ButtonAction()
{
	SpawnTab();
}

void FCustomSlateModule::SpawnTab()
{
	// 注册新 Tab
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("AliceTab"),
		FOnSpawnTab::CreateRaw(this, &FCustomSlateModule::OnSpawnTab))
	.SetDisplayName(FText::FromString(TEXT("Alice Tab")));

	// 显示新 Tab，注册后可以在任意地方这样做
	FGlobalTabmanager::Get()->TryInvokeTab(FName("AliceTab"));
}

TSharedRef<SDockTab> FCustomSlateModule::OnSpawnTab(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> Tab = SNew(SDockTab).TabRole(ETabRole::NomadTab);
	
	Tab->SetContent
	(
		SNew(SConstraintCanvas)
		+SConstraintCanvas::Slot()
		.Alignment(0.5f)
		.Anchors(0.5f)
		.Offset(FMargin(0.0f, 0.0f, 300.f, 100.f))
		.AutoSize(true)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Hello, Alice Tab !"))
		]
	);
	
	return Tab;
}
```




重新编译并重启编辑器，点击添加的按钮，就可以看到新 Tab 被打开了。

![](https://pic3.zhimg.com/v2-de5d4f2ff60b3779ac56fb05cbced152_b.gif)







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## 可布局的 Tabs ：TabManager

有一个可布局的窗口是一件非常酷的事情，它能够让用户自定义自己的工具窗口。接下来我们来探索如何生成可布局的 Tab 窗口。

来到 CustomSlate.h 中，添加新的入口函数和成员变量添加

```cpp
// CustomSlate.h

#pragma once

#include "Modules/ModuleInterface.h"

class FCustomSlateModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual ~FCustomSlateModule() {}

private:

	...

	/** 创建 LayoutTab */
	void SpawnLayoutTab();
	TSharedRef<SDockTab> FOnSpawnMainTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> FOnSpawnSubTab(const FSpawnTabArgs& Args, FText Text);
	
	TSharedPtr<FTabManager> TabManager;
	TSharedPtr<FTabManager::FLayout> TabManagerLayout;
	
};
```




接着来到 CustomSlate.cpp 中，我们首先将 SpawnLayoutTab() 添加到 ButtonAction() 中。

然后在 SpawnLayoutTab() 中注册要用到的 Tabs。并在生成 MainTab 之前先进行检查，如果 MainTab 已经存在，就将生成的操作变更为关闭当前打开的 MainTab。

接着来到 FOnSpawnMainTab() 中，在这里我们初始化生成布局的逻辑，并将其保存在成员变量“TabManagerLayout”中。每次当我们生成 MainTab 时，都重新从 TabManagerLayout 这个成员变量读取布局。

```cpp
// CustomSlate.cpp

...

#include "Widgets/Layout/SConstraintCanvas.h"

...

void FCustomSlateModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();

	// 在模块注销时注销
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FName("MainLayoutTab"));
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FName("SubTab.1"));
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FName("SubTab.2"));
}

void FCustomSlateModule::ButtonAction()
{
	SpawnLayoutTab();
}

void FCustomSlateModule::SpawnLayoutTab()
{
	// 注册 Tab
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("MainLayoutTab"),
		FOnSpawnTab::CreateRaw(this, &FCustomSlateModule::FOnSpawnMainTab))
	.SetDisplayName(FText::FromString("Layout Tab"));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("SubTab.1"),
		FOnSpawnTab::CreateRaw(this, &FCustomSlateModule::FOnSpawnSubTab,
			FText::FromString("This is Sub-Tab 1.")))
	.SetDisplayName(FText::FromString("SubTab 1"));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("SubTab.2"),
		FOnSpawnTab::CreateRaw(this, &FCustomSlateModule::FOnSpawnSubTab, 
			FText::FromString("This is Sub-Tab 2.")))
	.SetDisplayName(FText::FromString("SubTab 2"));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("SubTab.3"),
		FOnSpawnTab::CreateRaw(this, &FCustomSlateModule::FOnSpawnSubTab, 
			FText::FromString("This is Sub-Tab 3.")))
	.SetDisplayName(FText::FromString("SubTab 3"));

	// 查找已有Tab 防止反复生成Tab
	TSharedPtr<SDockTab> ExistingTab = 
		FGlobalTabmanager::Get()->FindExistingLiveTab(FName("MainLayoutTab"));
	
	if(!ExistingTab.IsValid())
	{
		// 如果没有已存在的Tab，生成Tab
		FGlobalTabmanager::Get()->TryInvokeTab(FName("MainLayoutTab"));
	}
	else
	{
		// 如果Tab已存在，关闭Tab
		ExistingTab->RequestCloseTab();
	}
}

TSharedRef<SDockTab> FCustomSlateModule::FOnSpawnMainTab(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> MainTab = SNew(SDockTab).TabRole(ETabRole::NomadTab);

	if(!TabManager.IsValid())
	{
		// 新建一个专用的TabManager，我们将用这个TabManager来恢复布局
		TabManager = FGlobalTabmanager::Get()->NewTabManager(MainTab);
	}

	if(!TabManagerLayout.IsValid())
	{
		// 生成布局
		TabManagerLayout = FTabManager::NewLayout("AliceTabManagerLayout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(.4f)
					->AddTab("SubTab.1", ETabState::OpenedTab)
					->SetHideTabWell(true)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(.4f)
					->AddTab("SubTab.2", ETabState::OpenedTab)
					->SetHideTabWell(true)
				)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(.4f)
				->AddTab("SubTab.3", ETabState::OpenedTab)
				->SetHideTabWell(true)
			)
		);
	}

	// 恢复布局
	TSharedRef<SWidget> TabContents = TabManager->RestoreFrom(
		TabManagerLayout.ToSharedRef(), TSharedPtr<SWindow>()).ToSharedRef();

	MainTab->SetContent
	(
		TabContents
	);

	return MainTab;
}

TSharedRef<SDockTab> FCustomSlateModule::FOnSpawnSubTab(const FSpawnTabArgs& Args, FText Text)
{
	TSharedRef<SDockTab> Tab = SNew(SDockTab).TabRole(ETabRole::NomadTab);

	Tab->SetContent
	(
		SNew(SConstraintCanvas)
		+SConstraintCanvas::Slot()
		.Alignment(0.5f)
		.Anchors(0.5f)
		.Offset(FMargin(0.0f, 0.0f, 300.f, 100.f))
		.AutoSize(true)
		[
			SNew(STextBlock)
			.Text(Text)
		]
	);

	return Tab;
}

// TODO:
// SaveLayout
```




重新编译并重启编辑器，点击添加的按钮，就可以看到新 Tab 被打开了。

![](https://picx.zhimg.com/v2-3ea54c3f617104be852939624591f865_b.gif)







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## FTabManager 简要归纳 [1]

☘  创建新的布局

```cpp
const TSharedRef<FTabManager::FLayout> DefaultLayout =FTabManager::NewLayout(LayoutName)
	->AddArea();
```




☘  添加可拖拽区域

```cpp
FTabManager::NewPrimaryArea();
```




☘  添加可拖拽分区

```cpp
FTabManager::NewSplitter();
```




☘  添加新的Tab

```cpp
FTabManager::NewStack();
```




☘  加载布局

```cpp
TSharedPtr<FTabManager::FLayout> UserLayout = FTabManager::FLayout::NewFromString(LayoutString
	.Replace(TEXT("("), TEXT("{")).Replace(TEXT(")"), TEXT("}")).Replace(TEXT("\\") LINE_TERMINATOR, LINE_TERMINATOR));
```




☘  保存布局

```cpp
//将Layout转成字符串
FString MyLayoutString = FGlobalTabmanager::Get()->PersistLayout()->ToString()
.Replace(TEXT("{"), TEXT("(")).Replace(TEXT("}"), TEXT(")")).Replace(LINE_TERMINATOR, TEXT("\\") LINE_TERMINATOR);

//通过GConfig或者其他方式可将其保存
GConfig->SetString(TEXT("MyLayouts"), *GI->TabManagerLayout.ToSharedRef()
->GetLayoutName().ToString(), *MyLayoutString, FPaths::ProjectConfigDir() / LayoutName);
GConfig->Flush(true, FPaths::ProjectConfigDir() / LayoutName);
```







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## 在 Slate 中使用 Widget

准备一个 UserWidget。新建 UserWidget 将其命名为“UMG_UsedInSlate”，并为它添加一些可显示的控件。

![](https://pic3.zhimg.com/v2-f1c5033133e32378c0b6fb7ca279e486_r.jpg)




来到“CustomSlate.cpp”中，在之前 SpawnTab 的函数 FOnSpawnTab() 下重写该函数。

```cpp
// CustomSlate.cpp

...
#include "Blueprint/UserWidget.h"

...

TSharedRef<SDockTab> FCustomSlateModule::FOnSpawnTab(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> Tab = SNew(SDockTab).TabRole(ETabRole::NomadTab);
	
	// 加载 UMG_UsedInSlate
	UClass* BlueprintClass = UEditorAssetLibrary::LoadBlueprintClass(FString(TEXT(
		"WidgetBlueprint'/Game/UMG_UsedInSlate.UMG_UsedInSlate'")));

	if(BlueprintClass)
	{
		TSubclassOf<UUserWidget> UserWidgetClass = BlueprintClass;
		UWorld* World = GEditor->GetEditorWorldContext().World();

		// 创建 UUserWidget
		UUserWidget* CreatedWidget = CreateWidget<UUserWidget>(World, UserWidgetClass);
	
		Tab->SetContent
		(
			SNew(SOverlay)
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				CreatedWidget->TakeWidget()  // 将 UUserWidget 转换为 SWidget
			]
		);
	}
	
	return Tab;
}
```




重新编译并重启编辑器，点击添加的按钮，就可以看到 UUserWidget 作为 SWidget 添加到 SOverlay 子部件了。

![](https://pica.zhimg.com/v2-758485f560fe0ae01a10ddd70639756a_b.gif)







$$
{♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\
$$







# 深入发掘的无限魅力：Slate 基础 ~ ♡ [2]




$隐藏着黑暗力量的钥匙啊，在我面前显现你真正的力量！ \\$ 




## Slate 概述

Slate 是一种完全自定义的、平台无关的用户界面架构。

UE的所有可视化工具都是基于 Slate UI 编写的, 而在游戏中推荐使用的 UMG UI 则是 Slate 的可视化编辑接口, 即在 Slate 的基础上又封装了一层可视化的接口。

Slate 的实质是利用宏和 C++ 运算符重载来实现的布局语法。

Slate 是基于事件和委托的, 也是一种 IMGUI。

Slate 为了做某件事而产生的工具集称作 ToolKit, 由下面元素组成。

- FSlateApplication -- Slate 的最高层管理器 -- 管理所有顶层窗口
- SWindow -- 顶层窗口 -- 针对不同平台对应 FGenericWindow
- Tab -- 标签页 -- 通过 TabManager 管理
- Dock -- 给 Tab 提供悬浮功能

除此之外, Slate 还为窗口准备了部局 Layout。

- Stack 类布局 -- 多个 Tab 共享一个区域, 通过 Tab 头切换
- Splitter 类布局 -- 可对本身的区域进行划分




Slate 只由三个大类组成，而后演化出了窗口中一切可以显示的东西。是不是有点道生万物而不自生的意味了呢。

具体来说，所有 Slate 都脱胎自三个 Parent Class，而这三个 Parent Class 都源自 SWidget。这就是最早的 Slate 了。




| Parent Class    | Children    | Exsamples                |
| --------------- | ----------- | ------------------------ |
| SLeafWidget     | None        | SImage, STextBlock       |
| SPanel          | Many        | SOverlay, SHorizontalBox |
| SCompoundWidget | One or more | SBorder, SButton         |







**SLeafWidget** 不带任何子控件，是 Slate 生长的终点，它通常是一些最终用于显示的非功能性内容，例如图片和文字。

**SPanel **所衍生出的 Slate 像画布一样，它们有各自不同的规则。SCanvas 能够允许其上的控件自由调整大小，SOverlay 允许控件之间相互堆叠，SHorizontalBox 帮助我们自动将控件水平排列，诸如此类。

**SCompoundWidget **是最常用的用于派生新 Slate 的类别。它有一个名为 ChildSlot 的受保护成员，允许我们在它上面悬挂其他子控件。




Slate 由两个相关模块组成，“SlateCore”模块和“Slate”模块。SlateCore 模块是 Slate 的核心框架，而 Slate 模块是 Slate 的库。所以，当我们要了解 Slate 是如何实现的，可以去到 SlateCore 中翻翻找找。而当需要一些 Slate 案例的时候，可以去到 Slate 中翻翻找找。

![](https://pic3.zhimg.com/v2-ae89ab0cd9ec0577faf16ccc2f67a4e0_r.jpg)




此外，如果你下载并完成编译了 UE5 源码版，那么可以找到一个叫做“SlateViewer”的示例项目，它将是一个很好的学习示例。

![](https://pic3.zhimg.com/v2-a5b3863796d2296b69268c3ae7dc3bba_r.jpg)

![](https://pic2.zhimg.com/v2-ec683ee244eb7b1fcb61c24460b72ff7_r.jpg)







$$
{♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\
$$







## Slate 的创建和入口

创建 Slate 有两种方式，SNew 和 SAssignNew。

SNew 返回 TSharedRef。在 .cpp 中想创建 Slate 临时引用或干脆直接生成并返回一个 Slate 时，使用起来会很方便。

```cpp
// 创建 STextBlock 并用引用接收
TSharedRef<STextBlock> TextBlockRef = SNew(STextBlock);
```

SAssignNew 返回 TSharedPtr。需要在 .h 中声明一个成员变量并在稍后赋值时，可以使用 SAssignNew。

```cpp
// 先声明，后赋值
TSharedPtr<STextBlock> TextBlockPtr;
SAssignNew(TextBlockPtr, STextBlock);
```




通常，我们使用 SNew 或 SAssignNew 构建 Slate，然后将其传给显示它们的入口即可。那么入口在哪里呢？

对于 SpawnWindow 示例来说，它可以是直接由 FSlateApplication 管理的 AddWindow。

```cpp
TSharedRef<SWindow> Window = SNew(SWindow)
...
// 直接由 FSlateApplication 管理的顶层入口
FSlateApplication::Get().AddWindow(Window);
```




对于 SpawnTab 示例来说，它可以是 FOnSpawnTab。

```cpp
void FCustomSlateModule::SpawnTab()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("AliceTab"),
		FOnSpawnTab::CreateRaw(this, &FCustomSlateModule::FOnSpawnTab)) // 代理函数入口
	.SetDisplayName(FText::FromString(TEXT("Alice Tab")));

	FGlobalTabmanager::Get()->TryInvokeTab(FName("AliceTab"));
}

TSharedRef<SDockTab> FCustomSlateModule::FOnSpawnTab(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> Tab = SNew(SDockTab).TabRole(ETabRole::NomadTab);
	
	...
	// 返回 TSharedRef<SDockTab>
	return Tab;
}
```




除此之外，每个 Slate 下的 Slot（如果存在）也是。随着探索的不断深入，我们会越来越接近 Slate 的底层，你也许会发现更多“入口”，但最终会发现它们都同宗同源。

![](https://picx.zhimg.com/v2-f650af84c39df324f4a46bf500cc2963_r.jpg)




$$
{♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\
$$







## 链式编程

链式编程是 Slate 的一种简化构建语法。一般 Slate 的链式编程看上去像这样。

![](https://pic3.zhimg.com/v2-c9143166762fdfa91e0e23a835cfb3d8_r.jpg)




当我们生成一个 Slate 控件，如果这个控件继承自 SCompoundWidget，你可以直接在生成的 Slate 控件后追加 [ ... ] 来填充它的 ChildSlot，你还可以在 [ ... ]前后通过“.xxx”来快捷地设置这个 Slate 控件的属性。

```cpp
SNew(SButton)
.SetSomething...
.SetSomething...
.SetSomething...
[
	SNew(STextBlock)
	.SetSomething...
	.SetSomething...
]
```




如果我们生成的 Slate 控件继承自 SPanel，那么你需要先增加 Slot，然后执行类似上面的操作。

```cpp
SlotSNew(SOverlay)
+SOverlay::Slot()
.SetSomething...
.SetSomething...
.SetSomething...
[
	SNew(SButton)
	.SetSomething...
	.SetSomething...
	[
		SNew(STextBlock)
		.SetSomething...
		.SetSomething...
	]
]
// 增加第二个Slot
+SOverlay::Slot()
.SetSomething...
.SetSomething...
.SetSomething...
[
	SNew(SButton)
	.SetSomething...
	.SetSomething...
	[
		SNew(STextBlock)
		.SetSomething...
		.SetSomething...
	]
]
// 增加第三个Slot
+SOverlay::Slot()
.SetSomething...
.SetSomething...
.SetSomething...
[
	SNew(SButton)
	.SetSomething...
	.SetSomething...
	[
		SNew(STextBlock)
		.SetSomething...
		.SetSomething...
	]
]
// 增加第N个Slot...  // Oppps! 真是太多了~~~♡
```




值得注意的是，Slate 的链式编程只是一种通过运算符重载实现的格式魔法，对于引擎默认提供的 Slate 控件而言，我们完全可以用正常的编程语句来替代它们。对于属性设置，下面的语句有相互对应的关系。

```cpp
SNew(STextBlock)
.Text(FText::FromString("Hello!"))

// same as ...
TSharedRef<STextBlock> TextBlock = SNew(STextBlock);
TextBlock->SetText(FText::FromString("Hello!"));
```




对于 Slot 来说，则看起来像这样。

```cpp
SNew(SButton)
[
	SNew(STextBlock)
	.SetSomething...
	.SetSomething...
]

// same as ...
TsharedRef<SButton> Button = SNew(SButton);
Button->SetContent
(
	SNew(STextBlock)
	.SetSomething...
	.SetSomething...
);

// or ...
TsharedRef<STextBlock> TextBlock = SNew(STextBlock);
TextBlock->SetText(FText::FromString("Hello!"));

TsharedRef<SButton> Button2 = SNew(SButton);
Button2->SetContent(TextBlock);
```




当然，我们也可以自定义属于自己的控件，并实现同样的事情，这是接下来将要探索的问题。







$$
{♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\
$$







## 自定义 Slate 控件

## Slate 控件构成

生成一个 Slate 控件类有一些固定的套路。理解 Slate 控件类的构成有助于我们在制作 Slate 控件类时有一个条理清晰的思路。通常一个 Slate 控件类看上去像这个样子。

![](https://pic4.zhimg.com/v2-1aaf43e1cdea36f2c320edf9a365a9c9_r.jpg)




首先它有一个固定的用于构造 FArguments 的组成部分。FArguments 是实现 Slate 语法的关键。（如果你完全不希望这个 Slate 构造类使用 Slate 语法，笔者甚至怀疑你可以将其去掉，但是我没有证据）。

由于 FArguments 的存在，类的 Construct 函数变得十分重要，它负责将 FArguments 中的东西取出来，将其储存到类自身的成员变量中来。通常 FArguments 中加入的变量，在该类自身或其父类中也应该有一个成员变量与其对应。

如果要制作的 Slate 控件类不打算作为已有控件类的拓展，而是想要有自己的显示效果，则需要重写 OnPaint 函数，ComputeDesiredSize 函数也需要做相应更新。如果一个类需要某些与输入相关的功能，则需要重写诸如 OnMouseButtonUp 等输入响应函数。

接下来是类成员变量，上面提到每个在 FArguments 中加入的变量，在该类自身或其父类中也应该有一个成员变量与其对应，通常我们也需要为这些变量制作 Set 函数和必要的 Get 函数。







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## FArguments 想要装起来

我们首先把 FArguments 捉来围观，它是整个 Slate 控件类中最特殊的地方。**FArguments 像一个大篓子，它是一个可以由外部传进数据，并被 Slate 控件类取走数据的地方**。

![](https://pica.zhimg.com/v2-3f5d47126eac15921d8d0fa3cbbdf98a_r.jpg)




制作 FArguments 的区域被两个名叫 SLATE_BEGIN_ARGS(){} 和 SLATE_END_ARGS() 的宏包裹起来。这两个宏的功能是用于制作一个 FArguments 结构体。

![](https://pica.zhimg.com/v2-eef7bc75a8085c730b14066bdc9cb470_r.jpg)

![](https://pica.zhimg.com/v2-0e4efe10e3b5cbe3dd554fc5165b2b10_r.jpg)




我们可以用一些宏来向这个 FArguments 结构体中填充一些内容。

- **SLATE_ARGUMENT( ArgType, ArgName )** 向 FArguments 中写入指定类型的参数。
- **SLATE_ATTRIBUTE( AttrType, AttrName )** 向 FArguments 中写入指定类型的属性。相比于参数只能是单纯的值，属性还可以绑定函数。
- **SLATE_EVENT( DelegateName, EventName )** 向 FArguments 中写入指定类型的代理。
- **SLATE_DEFAULT_SLOT( DeclarationType, SlotName )** 向 FArguments 中写入该 Slate 控件类的默认 Slot。




FArguments 是实现 Slate 链式编程语法的关键，接下来我们来看看在 FArguments 内部它都做了些什么。




## “ . ”是怎么来的：FArguments&

在使用 SNew() 或 SAssignNew() 创建 Slate 控件时，我们经常会使用以下的写法来直接设定这个控件的属性。

![](https://pic1.zhimg.com/v2-77bbd4e8c7848cff958f8a172778d5e2_r.jpg)




实际上，在使用 SLATE_ATTRIBUTE( AttrType, AttrName ) 等宏向 FArguments 中写入属性的时候，它会帮我们写一个与传入的“AttrName ”同名的函数，这个函数的返回值指向 FAguments 自身。

例如我们像这样使用 SLATE_ATTRIBUTE(float, SomeFloatAttrib) 写入一个名为 SomeFloatAttrib 的 float 属性。

![](https://pic4.zhimg.com/v2-fdca49ddb8137bf4fa424ea1217cb51d_r.jpg)




首先它将加入一个名为“_SomeFloatAttrib”的指定类型属性。 而后还写了“FArguments& SomeFloatAttrib( TAttribute<float> InAttribute )”函数。

![](https://pic2.zhimg.com/v2-ab310a16ae33d7c4325a2e6e5e24dc59_r.jpg)




当使用 SNew( WidgetType, ... ) 创建控件时，它返回一个该控件的 FArguments。

![](https://pic2.zhimg.com/v2-7e0255c114f780c4930e8d46b760ee51_r.jpg)




于是我们可以使用“.SomeFloatAttrib(...)”来修改“_SomeFloatAttrib”的值。修改完成后依旧返回 FArguments& 如此往复。







## “+”是怎么来的：SLATE_SLOT_ARGUMENT

如果使用 SNew() 创建诸如 SCanvas 这类继承自 SPanel 拥有多 Slot 的控件，我们可以使用像如下的方式来增加插槽。

![](https://pic3.zhimg.com/v2-a56e98335978e57881fd64764c1bb472_r.jpg)




来到 SCanvas 中，我们会看到一个新的宏。

![](https://pica.zhimg.com/v2-95b7aa492996750feab5bee2979f8a88_r.jpg)




这个宏向 FArguments 中加入了一个指定类型的 Slots 数组，并重载了“+”运算符。

![](https://pic1.zhimg.com/v2-7d3f374113bbfa673be76446e22680b2_r.jpg)




稍后，在 Construct() 中我们会取出这个数组。将这个数组添加到 Children 数组变量中。

![](https://picx.zhimg.com/v2-e11987891b5ed94556ec0aad205c2663_r.jpg)

![](https://pic1.zhimg.com/v2-0c328888da425996bd4a4e7b6cba5fcc_r.jpg)




FSlot 则是根据需要在类内部自己声明的。

![](https://picx.zhimg.com/v2-e5f80c6b322067a5c0fbeb9051b5299d_r.jpg)

![](https://picx.zhimg.com/v2-4bd096d2e964a04f03667d8a593a4f8d_r.jpg)




然后在 OnPaint() 中，对 Children 中的每个 Slot 进行处理。

![](https://pic1.zhimg.com/v2-522852043a4133a0b4f6f536811f06ae_r.jpg)







## “[ ]”是怎么来的：SLATE_DEFAULT_SLOT & SLATE_NAMED_SLOT

如果使用 SNew() 创建诸如 SButton 这类继承自 SCompoundWidget 拥有单 Slot 的控件，我们可以使用像如下的方式来使用插槽。

![](https://pica.zhimg.com/v2-fcb9d2b74827b3c17fdbdadf80759e72_r.jpg)




来到 SButton 中，我们会看到 SLATE_DEFAULT_SLOT() 宏。

![](https://pica.zhimg.com/v2-b9835c9eb187d905cbd3ae345b14b3a6_r.jpg)




这个宏调用了 SLATE_NAMED_SLOT() 宏，并重载了“[]”运算符。由于 SLATE_DEFAULT_SLOT() 宏里的这个重载方法，使得我们得以使用上面那样的 Slate 语法。

![](https://pic1.zhimg.com/v2-1cd75a0d289b841970a1ef87bbad58ac_r.jpg)




上面宏中的“[]”重载函数告诉我们，当使用诸如 SNew(SButton)[ SNew(STextBlock) ] 这样的语法时，我们将创建的 STextBlock 将保存在“_Content.Widget”中。“_Content”是在 SLATE_NAMED_SLOT() 宏中创建的一个 TAlwaysValidWidget 结构体。

![](https://picx.zhimg.com/v2-fc2187596ca855aa71f3ef9fdd81871f_r.jpg)




稍后在 Construct() 中可以将保存在 Slot 中的 STextBlock 取出来使用。

![](https://picx.zhimg.com/v2-cdea24488b2ec48f58985d73b7bdd0c3_r.jpg)




我们还可以使用 SNew(SButton).Content[ SNew(STextBlock) ] 这样的语法来做与 SNew(SButton)[ SNew(STextBlock) ] 相同的事情，因为在 SLATE_NAMED_SLOT() 有定义这样的与传入的 SlotName 同名的函数，在 SButton 中我们将其命名为了“Content”。

![](https://pica.zhimg.com/v2-a56a32df0734a6c37fc2d13bedde6080_r.jpg)




而它返回的 NamedSlotProperty 中也同样重载了“[]”运算符。

![](https://picx.zhimg.com/v2-b9c3abeb975719c11ea5ed35ffc1afc9_r.jpg)







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## SLATE_ARGUMENT 想要做自己

**SLATE_ARGUMENT( ArgType, ArgName )** 替我们向 FArguments 中添加一般类型的变量。

如果我们提供 SLATE_ARGUMENT( int32, SomeIntArg)，它就会在 FArguments 中添加如下变量。

![](https://pic2.zhimg.com/v2-183cacb264a1e18547d9b3f392f5c9b3_r.jpg)




这是 Slate 控件中最简单的变量形式，它用于那些无需绑定函数的普通变量，只管默默做好自己的本职储存工作，除此之外不关心外部的任何变化。

SLATE_ARGUMENT 之中由两个宏组成。
SLATE_PRIVATE_ARGUMENT_VARIABLE 将我们传入的内容转换为对应的原始类型变量。
SLATE_PRIVATE_ARGUMENT_FUNCTION 为这个变量提供一个与传入名同名的成员变量设置函数函数。

![](https://pic1.zhimg.com/v2-0ef17cae81b1654eb970e319604330fc_r.jpg)

它们转化为了一个对应的原始类型成员变量，变量名前以“_ArgName”表示，以及一个与“ArgName”同名的成员变量设置函数。

![](https://pic1.zhimg.com/v2-2faf1a845b232d7f9e471513b205dff6_r.jpg)







## SLATE_ATTRIBUTE 想要绑起来

**SLATE_ATTRIBUTE( AttrType, AttrName )** 替我们向 FArguments 中添加一种 TAttribute<ObjectType> 类型的变量。

如果我们提供 SLATE_ATTRIBUTE(float, SomeFloatAttrib)，它就会在 FArguments 中添加如下变量。

![](https://pic1.zhimg.com/v2-75147c99f3aad2515b6165139feb7ef4_r.jpg)




TAttribute<ObjectType> 内部由两个主要成员变量构成。Value 储存对应 ObjectType 的值，Getter 则可以绑定一个外部函数。




当 Getter 没有绑定任何内容时，我们可以将 TAttribute 视作一个单纯变量来使用。而当 Getter 被绑定后，Value 的值由每帧调用的 Getter 函数决定。

![](https://picx.zhimg.com/v2-7401ea4003aea17927765b1547708663_r.jpg)




在 TAttribute 中提供了一整套的完整捆绑方案，来供我们随意选用。（它真的好想被绑！）

![](https://pic2.zhimg.com/v2-d996ce6efdf7529f25ebf1f633e1f11d_r.jpg)




与 SLATE_ARGUMENT 类似，SLATE_ATTRIBUTE 内部也由两个宏组成。
SLATE_PRIVATE_ATTRIBUTE_VARIABLE 将我们传入的内容转换为对应的 TAttribute 类型变量。
SLATE_PRIVATE_ATTRIBUTE_FUNCTION 为这个变量提供一个与传入名同名的函数，以及一些用于绑定外部函数的绑定函数。

![](https://pic4.zhimg.com/v2-e85abb41a85aac3ad7ffcf9341429d21_r.jpg)




它们转化为了一个对应的 TAttribute 类型成员变量，变量名以“_AttrName”表示，以及一个与“AttrName”同名的成员变量设置函数，还有一些用于绑定外部函数的绑定函数。

![](https://pica.zhimg.com/v2-ce5ac88d65b194f1407ea1cfed34bf9a_r.jpg)







## SLATE_EVENT 想要发出去

**SLATE_EVENT( DelegateName, EventName )** 替我们向 FArguments 中添加事件代理。

如果我们提供 SLATE_EVENT(FOnClicked, SomeOnClickedEvent)，它就会在 FArguments 中添加如下变量。

![](https://pic3.zhimg.com/v2-e89e252200a35d1e78dff3c30ccddb58_r.jpg)




SLATE_EVENT 将输入的事件类型和名称转化为一个对应的事件类型的成员变量，变量名以“_EventName”表示，以及两个与“EventName”同名的成员变量设置函数，还有一些用于绑定外部函数的绑定函数。

![](https://pica.zhimg.com/v2-65c010a0ff2432b5aca1da62f74d6906_r.jpg)







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## Construct 想要拿进来

**如果 FArguments 是一个装进外部数据的大篓子，那 Construct 就是那个从这个篓子里拿走东西的人**。对于一个 Slate 控件类来说，它的构造函数可以写得放飞自我（不准飞），但是 Construct 函数一定要工工整整！

你可能要问为什么需要这么麻烦，还要弄出来两套一样的变量呢？当然是为了链式编程啦，基层的复杂诞生了上层的优美。也有可能含有某些笔者还不能参悟的深意，就像我家门前的那两棵枣树。




下面我们主要关注如何使用 Construct 将 FArguments 中的变量传递给 Slate 控件类中的变量。

![](https://picx.zhimg.com/v2-6b9135ea32342b791a2f7ce4e654271f_r.jpg)




首先来看我们的几个 Set 函数。

![](https://pica.zhimg.com/v2-1d15cea428f741928c398ee761387e26_r.jpg)




**☘  **对于 **SLATE_ARGUMENT **所代表的普通参数，我们只需要简单的赋值即可。

![](https://pic2.zhimg.com/v2-3f1235ef4c34054d601b1c45450816d3_r.jpg)




**☘ ** 对于 **SLATE_ATTRIBUTE **所代表的属性，在 Slate 控件类中声明时，我们有两种选择。
我们可以使用 TAttribute 类型，也可以使用 TSlateAttribute 类型。TSlateAttribute 是更新的，它使用缓存来解决 TAttribute 无条件调用 Getter.Execute() 所造成的性能问题，并且有一系列附加功能，能够让我们更精细地对 Attribute 进行操作。

如果我们声明 TAttribute 类型，那么直接等于 FArguments 中的属性就可以了。

```cpp
// SSampleWidget.h

TAttribute<float> SomeFloat;

// SSampleWidget.cpp
void SSampleWidget::Construct(const FArguments& InArgs)
{
	SomeFloat = InArgs._SomeFloatAttrib;  // 也可以写进 SetSomeInt() 函数里
}
```




但如果我们声明 TSlateAttribute 类型，我们必须在声明时指定失效类型，或者使用 SLATE_DECLARE_WIDGET 宏。我们将在稍后详细探讨 TSlateAttribute 的用法，现在只需知道它在声明时需要指定失效类型。

![](https://pic2.zhimg.com/v2-69068b378e369b28eae2d6a9aa563799_r.jpg)

然后我们就可以在 SetSomeFloat() 函数中用 TSlateAttribute 提供的绑定函数来绑定它。

![](https://pic3.zhimg.com/v2-d3bfe4d36a821ca92bd2ae593ad79c10_r.jpg)




对于 GetSomeFloat() 函数，我们使用 TSlateAttribute 或 TAttribute 的 Get() 方法即可。

![](https://picx.zhimg.com/v2-ee7586a3115a162c7f6601162011d219_r.jpg)




**☘  **对于 **SLATE_EVENT **所代表的事件类型，与 SLATE_ARGUMENT 一样直接赋值。

![](https://pica.zhimg.com/v2-06b2bd40c967fd99a573cd009951ef40_r.jpg)




**☘  SLATE_DEFAULT_SLOT **所代表的 Slot 则较为特殊，我们将其放入父类的 ChildrenSlot 即可。

![](https://pica.zhimg.com/v2-9952033ae94cb676e1e712dc3fa1a09e_r.jpg)




更加复杂的继承自 SPanel 的 Slate 控件类，则需要将子控件放入 OnPaint 中处理。







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## TSlateAttribute 与 TSlateManagedAttribute

在 Slate 中，TAttribute 针对开发人员效率进行了优化。它们使控件能够轮询数据，而不需要用户手动设置控件的状态。当性能不受关注时，TAttribute 通常工作良好，但当性能至关重要时（如游戏UI），TAttribute 就会成为瓶颈。

失效（Invalidation）系统仅允许已更改的控件执行昂贵的 Slate 布局。绑定的 TAttribute 与失效不兼容，因为我们不知道数据何时更改。此外，用于常见功能（例如可见性）的 TAttribute 每帧调用多次，仅此一项的委托开销就非常高。TAttribute 有很高的内存开销并且对缓存不友好。

TSlateAttribute 使属性系统可用于失效并更加的性能友好，同时保持 TAttribute 的好处不变。TSlateAttribute 在 Prepass 更新阶段每帧更新一次。 如果 TSlateAttribute 的缓存值发生变化，那么它将使控件失效。一旦控件失效，它就将被重新绘制。

成员属性按照变量在 SWidget 定义中定义的顺序更新（默认情况下）。托管属性以随机顺序更新（在 TSlateAttribute 之后）。成员属性的更新顺序可以通过设置先决条件来定义/覆盖（参见下面的示例）。

失效原因可以是一个谓词（参见下面的示例）。每个 SWidget 都可以覆盖失效原因。 使用 override 时要小心，因为它可以打破 widget 父级的失效。且仅当控件可见和未折叠时才会更新控件 TSlateAttribute。

TSlateAttribute 不可复制，只能存在于 SWidget 中。当 TSlateAttribute 是 SWidget 成员时，请使用它。当TSlateManagedAttribute 是数组或其他移动结构中的成员，并且数组是 SWidget 成员时，请使用它。如果不符合上述条件，对于其他所有内容，请使用 TAttribute。它们（TSlateAttribute 与 TSlateManagedAttribute）只能移动（不能复制），且消耗更多的内存。TSlateManagedAttribute 与 TSlateAttribute 一样快，但使用更多内存并且对缓存不太友好。

如果使用 TAttribute 来更改 SWidget 的状态，则需要重写 bool ComputeVolatility() const。
TSlateAttribute 和 TSlateManagedAttribute 不需要 ComputeVolatility()。

TSlateAttribute 请求一个 SWidget 指针。 应始终使用“this”指针。（SlateAttribute 指针保存在 SlateAttributeMetaData 中。当指针发生变化时，控件需要知道。）。像下面这样。

![](https://pic1.zhimg.com/v2-da7f8f606a1def073d53363039337cb0_r.jpg)

![](https://pic1.zhimg.com/v2-4af37c65031a842d9b4c957f840e6964_r.jpg)




**TSlateAttribute 示例**

![](https://picx.zhimg.com/v2-2bb35adea7f048cfae40d01c3fd3d9c1_r.jpg)






${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## OnPaint 快要忙死了 [4][5][6][7][8][9][10]

SWidget 的 OnPaint 类似于 蓝图的 Tick，仅在控件显示在屏幕上时每帧调用。

它让我们能够向屏幕上绘制图形，但它的内容实在太多了，我们在这里暂且放下，以后用专门的篇幅来介绍它。

$前方山河辽阔，我们改日再来 ... \\$ 

${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## ComputeDesiredSize 觉得不够大

ComputeDesiredSize 用于申请控件的所需空间，它对控件的排版很重要。但它的内容实在太多了，我们在这里暂且放下，以后用专门的篇幅来介绍它。

$前方山河辽阔，我们改日再来 ... \\$ 













![](https://pic4.zhimg.com/v2-ac116271a1b1fd849180954be47f3c99_r.jpg)


 

# 参考

1. [^](https://zhuanlan.zhihu.com/p/609002158#ref_1_0)FTabManager实现自定义可保存读取布局 [http://yanshuxin.xyz/?p=41](http://yanshuxin.xyz/?p=41)
2. [^](https://zhuanlan.zhihu.com/p/609002158#ref_2_0)UMG-Slate Compendium [https://github.com/YawLighthouse/UMG-Slate-Compendium#common-widgets](https://github.com/YawLighthouse/UMG-Slate-Compendium#common-widgets)
3. [^](https://zhuanlan.zhihu.com/p/609002158#ref_3_0)【ue4】【使用】插件开发 [https://cloud.tencent.com/developer/article/2145669](https://cloud.tencent.com/developer/article/2145669)
4. [^](https://zhuanlan.zhihu.com/p/609002158#ref_4_0)UE4 Onpaint画自定义顶点的三角形 [https://blog.csdn.net/qq_40756668/article/details/106824445](https://blog.csdn.net/qq_40756668/article/details/106824445)
5. [^](https://zhuanlan.zhihu.com/p/609002158#ref_5_0)UE4 C++OnPain函数的用法 [https://blog.csdn.net/qq_40756668/article/details/103070075](https://blog.csdn.net/qq_40756668/article/details/103070075)
6. [^](https://zhuanlan.zhihu.com/p/609002158#ref_6_0)UE 4.24 Slate合批机制剖析 [https://blog.uwa4d.com/archives/USparkle_UESlate.html](https://blog.uwa4d.com/archives/USparkle_UESlate.html)
7. [^](https://zhuanlan.zhihu.com/p/609002158#ref_7_0)UE4中UI解决方案Slate [https://zhuanlan.zhihu.com/p/24726208](https://zhuanlan.zhihu.com/p/24726208)
8. [^](https://zhuanlan.zhihu.com/p/609002158#ref_8_0)UE4 slate渲染流程 [https://zhuanlan.zhihu.com/p/528826488](https://zhuanlan.zhihu.com/p/528826488)
9. [^](https://zhuanlan.zhihu.com/p/609002158#ref_9_0)Unreal Engine slate 心得 [https://blog.csdn.net/wangyars/article/details/84490447](https://blog.csdn.net/wangyars/article/details/84490447)
10. [^](https://zhuanlan.zhihu.com/p/609002158#ref_10_0)UE4.25 Slate原始碼解讀 [https://tw511.com/a/01/46639.html](https://tw511.com/a/01/46639.html)