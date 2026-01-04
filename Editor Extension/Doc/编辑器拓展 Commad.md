> 《调教UE5》系列记录的是笔者在开发UE引擎中总结的些许经验。文中所有观点和结论仅代表个人见解，作为自学笔记和日后反思之参考，亦可能存在谬误或过时之处。如有错漏和不当，恳请多加指正。


---


[Helo：《调教UE5：编辑器拓展指南》开篇及索引](https://zhuanlan.zhihu.com/p/604684703)


---


# 目录

> **Input 的触发流程**
> **FUICommandInfo**
> **FBindingContext**
> **FInputBindingManager**
> **TCommands**
> **FUICommandList**
> **Command 基本流程**





---


$$
\large \textbf{飞鸟不知南北，它只去往春天。} \\
$$


---


在编辑器拓展的过程中，经常需要对各个模块的命令有一定的管理，以方便地组织和查找命令，以及避免命令之间的冲突。在 UE5 中，表示一个用户界面命令的类是 FUICommandInfo，它是命令的基本单元，TCommands 模板类是其“管理器”，它能够帮助我们方便地管理一组相关的 FUICommandInfo。

由于它们十分常见，也曾在拓展编辑器的探索过程中为笔者带来过一些小小的困惑，因此在这里单开一篇记录下对它们的一点探索。




目前，本指南基于 **UE5.1.0 **引擎版本进行编写，读者需前往下载对应的引擎版本，以免在尝试过程中因引擎版本差异造成不必要的麻烦。




# Input 的触发流程

![](https://pic1.zhimg.com/v2-afee14d5e0a932b1dbc887d6d0c3ca92_r.jpg)

当输入处理来到某个 SEditor 中，它会开始处理对这个输入的响应。以 SLevelEditor 为例，它会先将输入递交给自己的工具包（Toolkit）处理，由工具包检查自己持有的 CommandList 是否可以处理。如果传给工具包的输入没有被受理，则继续检查自己持有的 CommandList 是否可以处理。

![](https://pic1.zhimg.com/v2-22ae8d17919d7b72f8b9b7b327b40074_r.jpg)

在这个过程中涉及到 **FInputBindingManager**，它是 CommandInfo 的中心管理系统，所有的 CommandInfo 都以 FContextEntry 的形式储存在 FInputBindingManager 中，应用程序可以通过上下文（FBindingContext）和输入快捷键（FInputChord）来查找相应的 CommandInfo（`FInputBindingManager::FindCommandInContext()`）。这里有一个暗示，在同一个上下文中，所属 CommandInfo 的快捷键是不可以重复的。

FInputBindingManager 收集 CommandInfo 和 Context 的组织信息，但不关心 CommandInfo 背后的具体执行，它的功能类似于一个查询台，告诉发起查询的对象在特定的 Context 中有没有合适的 CommandInfo。




**CommandInfo **本身不包含具体的命令，它对应的是该命令的“特征”，包括快捷键、显示在菜单项或按钮上的图标、命令的名称等等。




![](https://pic2.zhimg.com/v2-f897659a3c720379f4f2b79a2a7ca8f1_r.jpg)

具体的命令由 **CommandList **提供，它储存一张 Map 表，成对储存 FUICommandInfo 和 FUIAction。同一 CommandList 中不允许存在相同 FUICommandInfo 的情况。而即使对于相同的 FUICommandInfo，在两个不同的 CommandList 中它们也可能对应不同的 FUIAction。

因此，FUICommandList 才是 Command 真正的“大本营”，它对于 Command 的行为有最终的决定权。当我们希望扩展某个 SEditor 中的快捷键命令时，首先要找到它使用的 FUICommandList，就像下面这样：

```cpp
// 要向 LevelEditor 中添加快捷键，先找到该模块使用的FUICommandList
FLevelEditorModule& LevelEditorModule = 
	FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
TSharedRef<FUICommandList> ExistingLevelCommands = 
	LevelEditorModule.GetGlobalLevelEditorActions();

// 将自己的FUICommandList附加进去
ExistingLevelCommands->Append(MyCommandList);
```










${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




# FUICommandInfo

**FUICommandInfo **是一个表示 **用户界面命令 **的类，它包含了命令的名称、描述、图标、快捷键等信息。

![](https://pica.zhimg.com/v2-422326b4b8072cb4c4ae4d8769dd7428_r.jpg)

|                   |                                                                                                                                                                            |
| ----------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Label             | 命令的默认显示名称，它会显示在菜单或工具栏上。                                                                                                                                                    |
| Description       | 命令的本地化帮助文本，它会显示在菜单或工具栏上的提示信息中。                                                                                                                                             |
| DefaultChords     | 命令的默认输入快捷键。                                                                                                                                                                |
| ActiveChords      | 命令的当前输入快捷键，在设置中由使用者自己设定的快捷键会记录在这里。                                                                                                                                         |
| Icon              | 命令的图标，它会显示在菜单或工具栏上。                                                                                                                                                        |
| UIStyle           | 命令的样式名称，暂时没有见到使用它的地方。                                                                                                                                                      |
| CommandName       | 命令的唯一标识符，用来区分不同的命令。                                                                                                                                                        |
| BindingContext    | 命令所属的输入绑定上下文的名称，通常用于表示命令的功能或用途，比如 LevelEditor，Viewport，ContentBrowser 等。BindingContext有两种获得方式，一种是从在生成FUICommandInfo时从构造函数的输入中获得，一种是在MakeCommandInfo时从输入的FBindingContext转换。 |
| Bundle            | 命令所属的捆绑组（这里暂时还没有解读明朗）。                                                                                                                                                     |
| UserInterfaceType | 命令的用户界面类型。                                                                                                                                                                 |




FUICommandInfo 中的关键函数是 **MakeCommandInfo()**，它构建一个 FUICommandInfo，并通过函数 `FInputBindingManager::Get().CreateInputCommand()` 在 FBindingContext 与 FUICommandInfo 之间建立联系，将它们储存在一个 **FContextEntry **中，这个 FContextEntry 会储存在 FInputBindingManager 名为 **ContextMap** 的变量里，由 FInputBindingManager 进行统一管理。

![](https://pic3.zhimg.com/v2-8ab70805df9a4db076eaf72c9eb8da88_r.jpg)

![](https://pic4.zhimg.com/v2-38999e37505818d5c5890fb8f3a3da37_r.jpg)




**ContextMap **是 FInputBindingManager 的数据库，FInputBindingManager 里大部分操作都是围绕 ContextMap 来进行的。FContextEntry 的内部元素表明，一个 FBindingContext（TSharedPtr< FBindingContext >）可以对应多个 FUICommandInfo（FCommandInfoMap），它们之间是一对多的关系。

![](https://pic1.zhimg.com/v2-327ff0d6c5f87fb4576d108209c4faf4_r.jpg)

通常，我们与 FInputBindingManager 交互的需求是获得一个 FUICommandInfo，然后拿着这个得到的 FUICommandInfo 去某个 FUICommandList 中查找对应的 Action：

```cpp
TSharedPtr<FUICommandInfo> Command = FInputBindingManager::Get().FindCommandInContext( Context, CheckChord, bCheckDefault );
const FUIAction* Action = SomeCommandList.GetActionForCommand(Command);
Action->Execute();
```







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




# FBindingContext

我们已经知晓 FBindingContext 与 FUICommandInfo 会关联在一起，且它们之间是一对多的关系。

**FBindingContext **是一个表示上下文的类，它用来管理一组相关的 FUICommandInfo。一个 FUICommandInfo 必须属于一个 FBindingContext，而一个 FBindingContext 可以包含多个 FUICommandInfo。

类似需要组织一个模块使用到的 Slate 资源、或者某一类通用 Slate 资源，需要 FSlateStyleSet 一样，组织一个模块使用到的 CommandInfo、或者某一类通用 CommandInfo，需要 FBindingContext。CommandInfo 记录自己所属的 FBindingContext，意在表明自己属于某一类用途。
FUICommandList 的 ContextsInList 变量中，记录了当前 CommandList 中所有 CommandInfo 所属的 Context，当需要根据 FInputChord 触发某个命令时，会到这些 Context 中寻找对应的命令。




> **点击式触发又如何呢？**
> 它们的 Action 通常被处理为直接和按键绑定起来，也就不需要再进行查找步骤了。而是在处理绑定的过程会执行查找的步骤。




FBindingContext 中包含了能够描述一个命令集合的相关信息：

![](https://picx.zhimg.com/v2-885a1a3a387dcabe0e41cb3d83074303_r.jpg)

|               |                                                                   |
| ------------- | ----------------------------------------------------------------- |
| ContextName   | 该 FBindingContext 管理的上下文名称                                        |
| ContextParent | 该 FBindingContext 所属的父级上下文名称                                      |
| ContextDesc   | 该 FBindingContext 管理的上下文描述，这个描述可以用来在用户界面中显示上下文的信息。                |
| StyleSetName  | 该 FBindingContext 所使用的样式集名称                                       |
| Bundles       | 该 FBindingContext 存储的命令包和它们的友好名称。命令包是一组相关的命令，它们可以被分配给一个快捷键或者一个菜单项 |




**ContextName **是 FBindingContext 最重要的属性，它可以通过 GetContextName() 函数取得。因此只要顺着 GetContextName() 关联用例来查找，我们就能够大概知道 FBindingContext 在如何运作。

首先，在 FInputBindingManager 中，ContextName 作为注册命令集时的操作依据。
其次，在 UI_COMMAND 宏内部，ContextName 与 FUICommandInfo 中的 CommandName 组合成“ContextName.CommandName”作为查找图标样式的名称：

![](https://pic4.zhimg.com/v2-a7518d0ad2beca245d4ddffa880c9be5_r.jpg)







> **Q：是否不同 FBindingContext 下的相同快捷键就一定不会冲突了？**
> A：不是。不同 FBindingContext 中的相同快捷键有可能相互冲突，具体来说，FInputBindingManager::Get().CreateInputCommand() 只检测相同 FBindingContext 或 FBindingContext 父族中的快捷键冲突，而没有检测不同 FBindingContext 之间的快捷键冲突。如果在多个 FBindingContext 中使用了相同的快捷键，那么当这些 FBindingContext 同时激活时，可能会导致不可预期的结果。

![](https://picx.zhimg.com/v2-d05d2ebc4a0646a08dccff593bbb8f25_r.jpg)







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




# FInputBindingManager

**FInputBindingManager** 负责创建和处理输入绑定。输入绑定是指将用户的输入（比如按键、鼠标、手柄等）映射到一些命令（比如移动、跳跃、攻击等）的过程。

FInputBindingManager 可以管理多个输入上下文（FBindingContext），每个输入上下文可以包含多个命令（FUICommandInfo），命令列表（FUICommandList）则可以将任意命令（FUICommandInfo）与执行操作（FUIAction）绑定在一起。因此，在使用 FUICommandInfo 时，也需要提供相应的 FUICommandList。随后当某个命令被触发，就会去到提供的命令列表中寻找相应的执行操作。

![](https://pic1.zhimg.com/v2-c51de747872ef1f2fbd6f65e0c0dada2_r.jpg)




FInputBindingManager 还可以通过 FOnRegisterCommandList 和 FOnUserDefinedChordChanged，让不同的系统之间共享和订阅命令列表，以及知晓快捷键变更的情况，以实现输入的一致性和灵活性。

![](https://pica.zhimg.com/v2-5206f7f54dcf17861cec649d9267dfa4_r.jpg)

![](https://pic2.zhimg.com/v2-af8b5e25d4b21703e9dd00351295bd1d_r.jpg)




FInputBindingManager 中的关键参数是 **ContextMap**，FInputBindingManager 里大部分操作都是围绕 ContextMap 来进行的。顺着 ContextMap  关联用例来查找，我们就能够大概知道 FInputBindingManager 在如何运作。

![](https://picx.zhimg.com/v2-f947c6c7b36f9ceb269c4c13465ff665_r.jpg)

![](https://pic4.zhimg.com/v2-978a9233fd90a7297ddf4d2429f0e043_r.jpg)

![](https://pica.zhimg.com/v2-e323d2e8a509a4c64af396d5c1c28e3a_r.jpg)

![](https://pica.zhimg.com/v2-b01dec25f2407d006774dfbb0373cdd8_r.jpg)

![](https://pic3.zhimg.com/v2-22498b99e891a3ddb7ca700501a30380_r.jpg)

![](https://pic4.zhimg.com/v2-f837a5d5299ca2aab9acb14f40139ba9_r.jpg)

![](https://pic3.zhimg.com/v2-8e314f6bffeb183f4876f076f430d6e8_r.jpg)

![](https://pica.zhimg.com/v2-25e246da8afecabcec1782be2f8cc532_r.jpg)

![](https://pic4.zhimg.com/v2-a14372ea6cc113fcbd5000f58c924483_r.jpg)

![](https://pic4.zhimg.com/v2-8725e2c84fe1e7012cca968e2fe3b46f_r.jpg)







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




# TCommands

**TCommands** 是一个模板类，它继承自 FBindingContext 类，用于创建和管理一组 FUICommandInfo 对象，以及与之相关的上下文和捆绑。

一般情况下，我们会为不同的功能或模块定义不同的 TCommands 类型，**并在其中创建和注册 FUICommandInfo 对象**。比如，编辑器中有一个 TCommands<FLevelEditorCommands> 类型，它用于定义和管理与关卡编辑器相关的命令。

使用 TCommands 的关键函数是 TCommands::**RegisterCommands**()，它在 TCommands::Register() 中被调用，之后由外部在需要注册 Commands 时调用 TCommands::Register()，一般是在模块初始化或准备使用该组命令时调用。因此在自定义自己的 TCommands 类时需要重写 TCommands::RegisterCommands()。

**UI_COMMAND **帮助将 FUICommandInfo 以 TCommands 自己当前的上下文注册到 FInputBindingManager 中，因此我们会看到 TCommands 的构造函数必须提供它的上下文信息，就像下面的伪代码所做的一样：

```cpp
class FExtendEditorCommands : public TCommands<FExtendEditorCommands>
{
	public:
	
	FExtendEditorCommands();

	virtual void RegisterCommands() override;

public:

	// FUICommandInfo List Here
	TSharedPtr< FUICommandInfo > SomeCommandInfo;
};

FExtendEditorCommands::FExtendEditorCommands()
	: TCommands<FExtendEditorCommands>(
		"MyCommands",                 // 需要提供上下文名称
		LOCTEXT( "My Commands", "My Commands" ),
		NAME_None,				// 需要提供父族上下文名称
		SomeStyleSet::GetStyleSetName())  // 需要提供样式集名称
{
}

void FExtendEditorCommands::RegisterCommands()
{
	// 注册 FUICommandInfo
	UI_COMMAND( SomeCommandInfo, "Command Label", "Command ToolTip", 
	EUserInterfaceActionType::Button, ThisCommandInputChord )
}

// 与 Action 绑定到 CommandList
TSharedRef<FUICommandList> MyCommandList = MakeShareable(new FUICommandList);
MyCommandList->MapAction(SomeCommandInfo, SomeAction);



// use it ...
TSharedPtr<FUICommandInfo> Command = FInputBindingManager::Get().FindCommandInContext( FExtendEditorCommands.GetContextName()/* or "MyCommands" */, CheckChord, bCheckDefault );
const FUIAction* Action = MyCommandList.GetActionForCommand(Command);
Action->Execute();
```







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




# FUICommandList

**FUICommandList **用于管理UI命令的执行、可执行性、可见性和选中状态等属性。它最明显的用途是将一组 FUICommandInfo 和 FUIAction 结对保存在列表中，以供后续命令触发时执行对应的操作。

![](https://pic2.zhimg.com/v2-20d996cdb3b480bcef57dd1561b6b845_r.jpg)




FUICommandList 内部的主要变量是 UICommandBindingMap 和 UICommandContextMap，分别对应 FUIAction 和 FUIActionContext。

**FUIAction **封装了一个UI操作的执行、可执行性、可见性和选中状态等属性，它可以通过不同的构造函数来初始化这些属性。
**FUIActionContext **是一个容器，它可以存储多个IUIActionContextBase类型的上下文对象，用于给FUIAction提供额外的数据，目前只有一个 IUIActionContextBase::GetContextName() 的可用函数。




`FUICommandList::MapAction()` 是 FUICommandList 最常用的函数，它将一个FUICommandInfo对象映射到一个FUIAction对象，或者一系列的委托函数。




其他一些比较重要的函数。

```cpp
// 处理由指定快捷键和键状态可激活的命令操作
bool ProcessCommandBindings( const FKey Key, const FModifierKeysState& ModifierKeysState, const bool bRepeat ) const;

// 尝试在当前UICommandList中查找指定命令的操作
const FUIAction* GetActionForCommand(TSharedPtr<const FUICommandInfo> Command) const;

// 尝试在当前UICommandList中查找指定命令的上下文
const FUIActionContext* GetContextForCommand(TSharedPtr<const FUICommandInfo> Command) const;

// 激活一个指定的 FUICommandInfo 命令操作
virtual bool ExecuteAction( const TSharedRef< const FUICommandInfo > InUICommandInfo ) const;

// 尝试激活一个指定的 FUICommandInfo 命令操, 如果 CanExecuteAction
bool TryExecuteAction( const TSharedRef< const FUICommandInfo > InUICommandInfo ) const;

// 检查是否可以执行与一个FUICommandInfo对象关联的操作
bool CanExecuteAction( const TSharedRef< const FUICommandInfo > InUICommandInfo ) const;

// 获取一个FUICommandInfo对象在工具栏中的可见性
EVisibility GetVisibility( const TSharedRef< const FUICommandInfo > InUICommandInfo ) const;

// 将另一个FUICommandList对象中的命令添加到当前的命令列表中
void Append( const TSharedRef<FUICommandList>& InCommandsToAppend );
```







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




# Command 基本流程

![](https://pic1.zhimg.com/v2-dfe36d9f2582dc411d245e0753a58878_r.jpg)

总结一下有关 Command 的基本流程，当我们需要一组新 Commands 时，就需要创建一个 TCommands 对象，并注册和管理一组新的 FUICommandInfo，这些 FUICommandInfo 一般保存在这个 TCommands 对象内部。

```cpp
class FExtendEditorCommands : public TCommands<FExtendEditorCommands>
{
public:
	
	FExtendEditorCommands();

	virtual void RegisterCommands() override;

public:

	// FUICommandInfo List Here
	TSharedPtr< FUICommandInfo > PrintLog;
};


class FExtendEditorActionCallbacks
{
public:
	// Action List
	static void PrintLog();
	
};


#define LOCTEXT_NAMESPACE "FExtendEditorCommands"
FExtendEditorCommands::FExtendEditorCommands()
	: TCommands<FExtendEditorCommands>(
		"ExtendEditorCommands",                 // 上下文名称
		LOCTEXT( "Extend Editor Commands", "Extend Editor Commands" ),
		NAME_None,
		FExtendEditorStyle::GetStyleSetName())  // 样式集名称，需要自己提供一个样式集
{
}


void FExtendEditorCommands::RegisterCommands()
{

	// 注册 FUICommandInfo
	UI_COMMAND(PrintLog, "Print Log Label", "Print Log ToolTip", 
	EUserInterfaceActionType::Button, FInputChord())

}
#undef LOCTEXT_NAMESPACE


void FExtendEditorActionCallbacks::PrintLog()
{
	UE_LOG(LogTemp, Warning, TEXT("StyleButtonAction is called."))
}
```




当需要使用这组命令时，先注册 TCommands 对象，然后将 FUICommandInfo 与要执行的操作绑定到 FUICommandList。

```cpp
// 当 FExtendEditorCommandModule 模块要使用 FExtendEditorCommands 时
void FExtendEditorCommandModule::StartupModule()
{
	FExtendEditorCommands::Register();
}

void FExtendEditorCommandModule::ShutdownModule()
{
	FExtendEditorCommands::Unregister();
}

TSharedRef<FUICommandList> CommandList = MakeShareable(new FUICommandList);
CommandList->MapAction(PrintLog, FExecuteAction::CreateStatic(&FExtendEditorActionCallbacks::PrintLog));
```




当需要触发一个 FUICommandInfo 时，我们也需要提供相应的 FUICommandList，以供查找可执行的命令和命令的执行方式。

```cpp
// ...
UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.User");
FToolMenuSection& Section = Menu->FindOrAddSection(NAME_None);

// 使用 Command 方式生成 ToolBarEntry
FToolMenuEntry ToolMenuEntry = FToolMenuEntry::InitToolBarButton(FExtendEditorCommands::Get().PrintLog);
// 为 ToolBarEntry 配置CommandList
ToolMenuEntry.SetCommandList(CommandList);

// 将配置好的 ToolBarEntry 添加到指定位置
Section.AddEntry(ToolMenuEntry);
```
















![](https://pic4.zhimg.com/v2-ac116271a1b1fd849180954be47f3c99_r.jpg)

