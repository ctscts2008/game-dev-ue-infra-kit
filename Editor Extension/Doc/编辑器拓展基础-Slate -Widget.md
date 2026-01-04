> 《调教UE5》系列记录的是笔者在开发UE引擎中总结的些许经验。文中所有观点和结论仅代表个人见解，作为自学笔记和日后反思之参考，亦可能存在谬误或过时之处。如有错漏和不当，恳请多加指正。


---


[Helo：《调教UE5：编辑器拓展指南》开篇及索引](https://zhuanlan.zhihu.com/p/604684703)


---


# 目录

> **不可忽略的事前准备：新建 CustomSlate 模块 ~ **♡
> **ㅤ创建 CustomSlate 模块**
> **ㅤ在 ToolBar 栏上创建新按钮**
> **即取即用的便捷体验：EditorUtilityWidget 篇 ~ **♡
> **ㅤWidget 与 Slate**
> ㅤ**Editor Utility Widget**
> **ㅤ自定义 UWidget**
> **ㅤ通过 C++ 打开 Editor Utility Widget**
> **ㅤ通过蓝图打开 Editor Utility Widget**


---


$$
\large \textbf{没有难以传达的心意，只有不合时宜的相遇。} \\
$$


---


本章指南是 [《调教UE5：编辑器拓展指南》Slate 相关基础](https://zhuanlan.zhihu.com/p/609002158) 的姊妹篇，如果你已经阅读过该章内容，可直接前往本章“即取即用的便捷体验：EditorUtilityWidget 篇 ~ ♡”部分，进行后续阅读。


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

```text
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

```text
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

```text
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

```text
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




# 即取即用的便捷体验：EditorUtilityWidget 篇 ~ ♡




$“Widget究竟哪里好呢？”\\ “它就像你口袋里的玫瑰清口糖。”\\$ 




## Widget 与 Slate

Widget 是 UE5 中用于对界面进行可视化编辑的工具，它继承自 UWidget 类。在本文中如无特殊说明，“Widget”一般都默认指代 UWidget 类。

与 Widget 相对应的就是 Slate 了，Slate 继承自 SWidget 类。不严谨但有助于快速理解地类比来说，Widget 之于 Slate，就像 Blueprint 之于 C++ 的关系。

我们不仅可以使用引擎提供的 Widget，还可以自己通过 Slate 来设计自己需要的 SWidget，然后将其转化为 UWidget 来供 Widget 系统使用。虽然大部分时候引擎提供的 Widget 已经足够满足需求了。

Widget 的优势在于可视化的界面编辑，所见即所得让界面设计变得很容易，且易于迭代。Slate 的优势自不必多言，Widget 也是站在其肩膀之上建设起来的，它是我们万丈高楼的基本构成。

因此，掌握 Slate 就能够获得自由编辑引擎窗口的能力。当然，就和所有偏向底层的功能一样，Slate 使用起来并不像 Widget 那样容易，想要将它运用自如需要一些探索。




## Editor Utility Widget 简易说明

Editor Utility Widget（以下简称为“EUW”）也是一种 Widget。如果想要修改虚幻编辑器的用户界面，我们可以使用 EUW 来添加新的 UI 元素。EUW 是基于 UMG 的，所以我们可以像在任何其他 UMG 控件蓝图中一样设置 EUW 中的控件。

① 单击右键，选择 Editor Utilities > Editor Utility Widget，我们可以创建一个 EUW。

![](https://picx.zhimg.com/v2-7e357e29eefe11b8e40d9f05c71d68bf_r.jpg)

② 双击打开创建好的 EUW，将 Text 控件拖拽到 Canvas Panel 中。

![](https://pic2.zhimg.com/v2-0c620f705243cc716e08619f891badc3_r.jpg)

③ 选中 Text 控件，修改锚点信息。

![](https://pic4.zhimg.com/v2-e89bdfe0563d6cf6f7bcaa5ed5e3c0bd_r.jpg)

④ 然后修改对应设置。

![](https://pica.zhimg.com/v2-03a1e359c5809fc5b3afa75c6b8c7a20_r.jpg)

⑤ 修改完成后点击编译和保存关闭 Widget 编辑器。在 ContentBrowser 中右键单击 EUW 资产，选择“Run Editor Utility Widget”。

![](https://pic2.zhimg.com/v2-4ba73eab0d5a97a596b7a485a3cc0c13_r.jpg)

![](https://pic1.zhimg.com/v2-e14941125b155d9053a8748cb76d8b34_r.jpg)




一个简单的编辑器 Tab 面板就生成了。

![](https://pic1.zhimg.com/v2-96c992e65f687c2430e388138a7c9fc6_r.jpg)




我们可以将它挪到任何可以停靠 Tab 的地方，就像其他的编辑器 Tab 面板那样。

![](https://pic1.zhimg.com/v2-3e475ca2cb7e5e7706bb27a959616cc2_r.jpg)




相比于 Slate，Widget 更加易于迭代且无需频繁地进行编译。对于开发人员来说，其可视化的编辑方式也是用来缩短开发周期的最佳法宝。

![](https://pic3.zhimg.com/v2-e54649dae8fd482af838bffb6c6d80ec_r.jpg)







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## 自定义 Widget

能够将制作好的 Slate 转化为 Widget 可用的控件，在灵活运用 EUW 进行编辑器拓展时是十分必要的。下面来介绍自定义 Widget 的方法。

要制作 Slate 的 Widget 版本，需要将 SWidget 类转换为 UWidget 类。

此处我们将 STextBlock 转换为 UCustonWidget。

① 新建 UWidget 子类。在 CustomSlate 文件夹下新建如下文件。

- CustomSlate 文件夹
- CustomWidget.h
- CustomWidget.cpp

![](https://pica.zhimg.com/v2-1eb260bcfcde0ef0a9cfa6e5b79b32de_r.jpg)




② 在 CustomWidget.h 中，我们需要重写三个父类函数。

- **RebuildWidget** 用于构建该 UCustomWidget 所持 SWidget 类的内容。
- **ReleaseSlateResources **用于释放 UCustomWidget 所持 SWidget 类的指针。
- **SynchronizeProperties **用于更新 UCustomWidget 所持 SWidget 类的内容。

```cpp
// CustomWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"

#include "CustomWidget.generated.h"

UCLASS()
class UCustomWidget : public UWidget
{
private:
	GENERATED_BODY()

public:

	/** 释放 MyCustomWidget */
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	/** 更新 MyCustomWidget */
	virtual void SynchronizeProperties() override;

protected:

	UPROPERTY(EditAnywhere)
	FText CustomText;

	/** 构建 MyCustomWidget */
	virtual TSharedRef<SWidget> RebuildWidget() override;

	TSharedPtr<STextBlock> MyCustomWidget;
};
```




③ 在 CustomWidget.cpp 中分别实现这三个函数。

**此处我们为了简化演示仅实现了 Text，在实际使用中应尽量将 STextBlock 的所有属性都实现一遍。**

```cpp
// // CustomWidget.cpp

#pragma once

#include "CustomWidget.h"

void UCustomWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyCustomWidget.Reset();
}

void UCustomWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if(MyCustomWidget)
	{
		MyCustomWidget->SetText(CustomText);
	}
}

TSharedRef<SWidget> UCustomWidget::RebuildWidget()
{
	MyCustomWidget = SNew(STextBlock)
	.Text(CustomText);

	return MyCustomWidget.ToSharedRef();
}

```




④ 在 CustomSlate.Build.cs 中，添加模块依赖项。

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
			"Slate",
			"UMG",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
```




⑤ 编译并重新打开 Editor，打开 Editor Utility Widget 编辑器。就可以看到我们的 CustomWidget 控件了。







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## 通过 C++ 打开 Editor Utility Widget

通过 C++ 打开 EUW 的能力对于界面开发来说十分重要。虽然它可能不太优雅，但却十分实用。

来到 CustomSlate.h 中，添加运行  Editor Utility Widget 的入口函数。

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
	
	/** 运行EUW入口函数 */
	void RunEditorUtilityWidget();
};
```




将入口函数加入 ButtonAction() 并实现。我们采用的是直接从 EUW 资产引用持有 UObject 的方式。要获取资产的引用，可以通过右键菜单。

![](https://pic4.zhimg.com/v2-fbf9d4816d3c235b1ebb187d5bed3ff5_r.jpg)

```cpp
// CustomSlate.cpp

...
#include "EditorAssetLibrary.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "EditorUtilitySubsystem.h"

...
void FCustomSlateModule::ButtonAction()
{
	RunEditorUtilityWidget();
}

void FCustomSlateModule::RunEditorUtilityWidget()
{
	UObject * Blueprint = UEditorAssetLibrary::LoadAsset(FString(TEXT(
		"EditorUtilityWidgetBlueprint'/Game/EUW_MyWidget.EUW_MyWidget'")));

	if(Blueprint)
	{
		UEditorUtilityWidgetBlueprint* EditorWidget = 
			Cast<UEditorUtilityWidgetBlueprint>(Blueprint);
		
		if(EditorWidget)
		{
			UEditorUtilitySubsystem* EditorUtilitySubsystem = 
				GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();

			EditorUtilitySubsystem->SpawnAndRegisterTab(EditorWidget);
		}
	}
}
```




重新编译并重启编辑器，点击添加的按钮，就可以看到 EUW 被运行了。

![](https://pic3.zhimg.com/v2-b2d72bef47fb54524c3e70c3919b57fc_b.gif)




${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## 通过蓝图打开 Editor Utility Widget

我们新建一个 EUW，将其命名为“EUW_RunEUW”，我们通过这个 EUW 来打开之前创建的 Editor Utility Widget。

双击打开 EUW_RunEUW，为其添加一个按钮，并为按钮添加 Text。然后选中按钮，在右侧细节面板中的“OnClick”事件后点击“+”创建事件。

![](https://pic2.zhimg.com/v2-61fade7cda8e858d27d52f44847949c9_r.jpg)




来到事件图表中，单击右键查找“Get Editor Utility Subsystem”，添加 EditorUtilitySubsystem 节点。




按照如下设置连接蓝图节点。

![](https://pic1.zhimg.com/v2-a26a68a1093f4730228ed4b99e1d2792_r.jpg)




编译并保存后，关闭 Widget 编辑器。右键运行 EUW_RunEUW。点击按钮，打开 EUW_MyWidget。

![](https://pic2.zhimg.com/v2-ea9a23f361d5ff738ab84e9e75b96fcd_b.gif)







![](https://pic4.zhimg.com/v2-ac116271a1b1fd849180954be47f3c99_r.jpg)

