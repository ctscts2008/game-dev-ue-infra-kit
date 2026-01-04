> 《调教UE5》系列记录的是笔者在开发UE引擎中总结的些许经验。文中所有观点和结论仅代表个人见解，作为自学笔记和日后反思之参考，亦可能存在谬误或过时之处。如有错漏和不当，恳请多加指正。


---


[Helo：《调教UE5：编辑器拓展指南》开篇及索引](https://zhuanlan.zhihu.com/p/604684703)


---


# 目录

> **UToolMenus**
> **ㅤUToolMenus 体系概述**
> **ㅤMenu 管理（UToolMenus）**
> **ㅤㅤRegisterMenu() 与 ExtendMenu()**
> **ㅤㅤGenerateWidget()**
> **ㅤMenu（UToolMenu）**
> **ㅤㅤUToolMenu 结构**
> **ㅤㅤUToolMenu 主要函数**
> **ㅤSection（FToolMenuSection）**
> **ㅤMenuEntry（FToolMenuEntry）**
> **Builder**
> **ㅤUToolMenus 体系中 Builder 的运作原理**
> **ㅤFExtender，FExtensibilityManager，FToolMenuContext 与 Builder**
> **ㅤ单独使用 Builder**


---


$$
\large \textbf{古有小窃者，掩耳可窃铃} \\ \large \textbf{今有剧盗者，失业率不明} \\ \large \textbf{埋头行大事，食言多自肥} \\ \large \textbf{宁可断脊喉，不敢认功勋} \\
$$


---


# UToolMenus

**UToolMenus **是UE5编辑器中用于创建菜单和工具栏的工具。本文主要介绍与 UToolMenus 相关的概念，探索UE5编辑器创建菜单和工具栏的流程，以帮助对自定义菜单和工具栏有需要的开发者快速了解 UToolMenus 全貌。

## UToolMenus 体系概述

![](https://picx.zhimg.com/v2-084a2a38df3ee6391fc97ef93f7044d5_r.jpg)

**UToolMenu **是菜单的主体，它的管理者是 **UToolMenus**，UToolMenus 负责 UToolMenu 的注册和记录，它有一个名为 Menus 的 TMap 成员变量，用于收纳已注册或未注册的 UToolMenu。我们的最终目的是将 UToolMenu 生成为一个 SWidget 以供某个界面显示，这个过程由 UToolMenus 中的 **GenerateWidget()** 函数发起，它会根据传入 UToolMenu 的 MenuType 创建合适的 **Builder**，将 UToolMenu 生成为一个 SWidget。

我们本次的探索有两个部分，一部分围绕 UToolMenus 如何组织 UToolMenu 来进行，另一部分则围绕 Builder 如何将 UToolMenu 生成为 SWidget 来进行。




${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## Menu 管理（UToolMenus）

在UE5编辑器中，根据菜单生成的方式，可以分为 **UToolMenu **生成和 **FExtender **生成两种方式。UToolMenu 生成的是相对固定的菜单，这种菜单原则上不经常变动，例如编辑器原生的菜单。FExtender 生成的是动态的菜单，例如插件或一般模块添加的菜单，以及需要根据上下文情况发生变化的菜单。原则上如此，但并非是说 UToolMenu 只能生成固定的菜单，UToolMenu 也有动态生成菜单的能力，甚至有在蓝图中配置菜单的功能。

UToolMenu 与 FExtender 根本上的不同在于它们的配置时机。之前我们提到 Builder 会将传入的 UToolMenu 生成为 SWidget，而 UToolMenu 的菜单结构是在传入 Builder 之前就已经被配置好的，Builder 只会根据 UToolMenu 的菜单结构生成 SWidget，它不会更改这个结构，这也是我们说 UToolMenu 生成的是相对固定的菜单的原因。然后，Builder 会将 FExtender 需要生成的菜单项目根据 Hook 信息插入到 UToolMenu 的菜单中，因此，FExtender 的配置是在生成时的，而 UToolMenu 的配置是在生成前的。

![](https://pic1.zhimg.com/v2-ff1a0b4af8dfb87fbf07304136a6177c_r.jpg)

**UToolMenus **是一个全局单例，它负责管理所有UE5编辑器可能要生成的 UToolMenu，即配置 UToolMenu 的部分。它有一个名为 Menus 的 TMap 用来记录这些 UToolMenu，我们可以看到 UToolMenus 中的一些关键成员函数都与这个成员变量有关。

一个 UToolMenu 的结构由两个层次组成：**Section **和 **Entry**。UToolMenu 的实际内容是 Entry，Section 是 Entry 的分组和插入点。




## + RegisterMenu() 与 ExtendMenu()

![](https://pica.zhimg.com/v2-aabe8fbf400577964ed1205f7bdbbcd2_r.jpg)

UToolMenus 中有两个十分相似的函数：RegisterMenu() 和 ExtendMenu()。ExtenderMenu() 的本意是搜寻存在的 UToolMenu 并加以拓展，但实际它的用法十分的宽松，甚至可以用于添加原本不存在的 UToolMenu，这使得在一般情况下，单参数的 RegisterMenu() 和 ExtendMenu() 几乎可以完成同样的工作，例如下面的代码，两个函数实现的功能是相同的：

```cpp
UToolMenu* ExsitingMenu = UToolMenus::Get()->FindMenu("LevelEditor.MainMenu");

FToolMenuSection& ExsitingSection = ExsitingMenu->FindOrAddSection(NAME_None);

FToolMenuEntry& NewSubMenuEntry =
ExsitingSection.AddSubMenu(
	"NewMenu",
	LOCTEXT("New Menu", "New Menu"),
	LOCTEXT("", ""),
	FNewToolMenuChoice()
	);

MakeEntry.InsertPosition = FToolMenuInsert("Help", EToolMenuInsertType::After);

static const FName NewMenuName = "LevelEditor.MainMenu.NewMenu";
UToolMenu* NewMenu = UToolMenus::Get()->RegisterMenu(NewMenuName);

// ExtendMenu() 在此时与 RegisterMenu() 类似
// UToolMenu* NewMenu = UToolMenus::Get()->ExtendMenu(NewMenuName);

FToolMenuSection& NewSection = NewMenu->AddSection("New Section", FText::FromString("New Section"));
	
NewSection.AddMenuEntry(
	"NewMenuEntry",
	LOCTEXT("New Menu Entry", "New Menu Entry"),
	LOCTEXT("", ""),
	FSlateIcon(),
	FToolUIActionChoice()
	);
```

这里的 NewSubMenuEntry 是一个 Entry，而 LevelEditor.MainMenu.NewMenu 是一个 UToolMenu，它们是如何关联起来的呢？

![](https://picx.zhimg.com/v2-8fa66657eae58a651547772e3bd46dc9_r.jpg)

在使用 UToolMenus::GenerateWidget() 生成为 SWidget 时，函数会将传入的 UToolMenu 里的 Entry 逐个填充到与 UToolMenu 菜单类型（MenuType）对应的 Builder 中，LevelEditor.MainMenu 是一个 MenuBar 类型的 UToolMenu，因此要使用对应的 PopulateMenuBarBuilder() 函数来执行填充。实际上，这里有一个配置 Entry 点击后生成下拉菜单所需的 UToolMenu 的过程。在上面的代码示例中，我们有如下的代码：

```cpp
ExsitingSection.AddSubMenu(
	"NewMenu",
	LOCTEXT("New Menu", "New Menu"),
	LOCTEXT("", ""),
	FNewToolMenuChoice()
	);
```

其中函数的第四个参数允许我们提供自己的生成函数，比如我们像下面这样使用：

```cpp
ExsitingSection.AddSubMenu(
	"NewMenu",
	LOCTEXT("New Menu", "New Menu"),
	LOCTEXT("", ""),
	FNewToolMenuChoice().NewMenuLegacy.CreateLambda(...)
	// or :
	// FNewToolMenuChoice().OnGetContent.CreateLambda(...)
	);
```

但如果 FNewToolMenuChoice() 不做任何设置，则会生成一个默认的代理函数，这个代理函数将创建一个新的 UToolMenu，它的名称由当前 Entry 所属的 UToolMenu 的名称，以及 Entry 的名称共同组成：

```cpp
FName UToolMenus::JoinMenuPaths(const FName Base, const FName Child)
{
	return *(Base.ToString() + TEXT(".") + Child.ToString());
}
```

UToolMenus::GenerateSubMenu() 会在 UToolMenus 的各种记录中寻找与新建 UToolMenu 同名或父菜单里同名的 UToolMenu，并收集它的层次结构，复制给新建的 UToolMenu。例如，LevelEditor.MainMenu 有父菜单 MainFrame.MainMenu，因此会搜寻是否存在名为 LevelEditor.MainMenu.NewMenu 和 MainFrame.MainMenu.NewMenu 的 UToolMenu。如果存在，就收集它们的层次结构，复制给新建的 UToolMenu。

如此，通过将原有 UToolMenu 复制到临时 UToolMenu 并提交到 Builder 的方式，将 NewSubMenuEntry 与 LevelEditor.MainMenu.NewMenu 关联到一起。这正是为什么我们在 NewMenu 中添加的内容可以出现在 NewSubMenuEntry 下拉菜单中的原因。




![](https://pic4.zhimg.com/v2-a624ec4a013b658cc3fe958a515b07e7_r.jpg)

相比于 ExtendMenu()，RegisterMenu() 有更多参数，其中最值得关注的是 InParent，这个参数用于确定注册 UToolMenu 的父子关系。一个 UToolMenu 能够继承来自父菜单的结构，并在此基础上进行扩展，其行为类似于类的继承关系。

下面的代码示例中 LevelEditor.MainMenu.HelpChildren 继承了 LevelEditor.MainMenu.Help，因此菜单  HelpChildren 可以拥有所有 Help 菜单的内容。

![](https://pic4.zhimg.com/v2-9fc3309ed26bd415545c3b123b53904b_r.jpg)

```cpp
UToolMenu* ExsitingMenu = UToolMenus::Get()->FindMenu("LevelEditor.MainMenu");
FToolMenuSection& ExsitingSection = ExsitingMenu->FindOrAddSection(NAME_None);

FToolMenuEntry& HelpChildrenEntry =
ExsitingSection.AddSubMenu(
	"HelpChildren",
	LOCTEXT("HelpChildren", "Help Children"),
	LOCTEXT("", ""),
	FNewToolMenuChoice()
	);

HelpChildrenEntry.InsertPosition = FToolMenuInsert("Help", EToolMenuInsertType::After);

static const FName HelpChildrenName = "LevelEditor.MainMenu.HelpChildren";
// 将 LevelEditor.MainMenu.Help 作为父菜单
UToolMenu* HelpChildrenMenu = UToolMenus::Get()->RegisterMenu(HelpChildrenName, "LevelEditor.MainMenu.Help");

FToolMenuSection& NewSection = HelpChildrenMenu->AddSection("New Section", FText::FromString("New Section"));
NewSection.AddMenuEntry(
	"ChildrenButton",
	LOCTEXT("ChildrenButton", "Children Button"),
	LOCTEXT("", ""),
	FSlateIcon(),
	FToolUIActionChoice()
	);
```




注意，这里的“父子关系”不是 LevelEditor.MainMenu<=>LevelEditor.MainMenu.Help 这种，而是 LevelEditor.MainMenu<=>MainFrame.MainMenu 或 LevelEditor.MainMenu.Help<=>LevelEditor.MainMenu.HelpChildren 这种，前者通过命名表明的是层级关系，而非继承关系。




## + GenerateWidget()

从 UToolMenu 生成菜单是一个复杂的过程，其中的核心思路不难，但实现起来涉及到的细节繁多。篇幅所限，我们在这里仅梳理生成菜单的核心思路。

![](https://picx.zhimg.com/v2-e312ef7883ed7fad425870d130226dbf_r.jpg)

通常我们需要提供一个菜单名，以生成对应的菜单控件，这个函数主要执行两步操作：

① 根据提供的菜单名收集信息，创建对应的 UToolMenu。
② 将梳理好的 UToolMenu 提交给 GenerateWidget() 生成菜单控件。

在第 ① 步中，如果菜单有父子层次，则会收集这些内容，生成完整的 UToolMenu。经过这个步骤后， UToolMenu 的结构虽然是固定的，但在这个收集的过程中是可以增加动态内容的，这里有两处地方：

![](https://pica.zhimg.com/v2-4559cd42deb7daf862cc24c1f35e06f8_r.jpg)

第一处位于 UToolMenus::AssembleMenu() 函数之中，动态内容由 FToolMenuSection 中的 **ToolMenuSectionDynamic **或 **NewToolMenuDelegate **代理提供。这里提供的是 Section 的动态构造，ToolMenuSectionDynamic 可用于蓝图，NewToolMenuDelegate 则是纯粹的 C++ 内容。

![](https://pic1.zhimg.com/v2-47dae0808a413297728cee17da80a8a8_r.jpg)

第二处在菜单组装完成之后，有 UToolMenus::ApplyCustomization() 函数对已组装菜单进行定制化。定制化可以控制 UToolMenu 中 Section 和 Entry 的显示和隐藏，以及它们的排序。




![](https://pic2.zhimg.com/v2-383679ee4b914f900683051766d3a4a7_r.jpg)

UToolMenu 构造完成后，就被送往第 ② 步中的 Builder 进行填充（Populate）。首先 UToolMenus::GenerateWidget() 会根据 UToolMenu 的菜单类型（MenuType）选择创建对应的 Builder，菜单类型分为四种：

- MenuBar
- Menu
- ToolBar
- SlimHorizontalToolBar

所谓的“填充”，是在将 UToolMenu 中的结构内容转换为对应 Builder 中 **MultiBox **的结构内容，最终 Builder 再拿着这个 MultiBox 去生成 SWidget。







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## Menu（UToolMenu）

![](https://pic1.zhimg.com/v2-81cc16323b184dd059bfef6775411fca_r.jpg)

**UToolMenu **是菜单的主体，它用于记录一个菜单控件的结构。




## + UToolMenu 结构

![](https://pic3.zhimg.com/v2-532aea685226e92deb6e6d0043f2fada_r.jpg)

UToolMenu 的结构由两个层次组成：**Section **和 **Entry**。UToolMenu 的实际内容是 Entry，Section 是 Entry 的分组和插入点。

这里的 HISTORY，EDIT，CONFIGURATION 是 Edit 菜单中的 Section，具体的按钮则是 Entry。Entry 必须属于一个 Section 之下，这样才能确定它们在菜单中出现的位置，一种特殊情况是不具名的 Section，例如下面这样的写法：

```cpp
UToolMenu* ExsitingMenu = UToolMenus::Get()->FindMenu("LevelEditor.MainMenu");

// 不具名 MenuSection
FToolMenuSection& ExsitingSection = ExsitingMenu->FindOrAddSection(NAME_None);

FToolMenuEntry& HelpChildrenEntry =
ExsitingSection.AddSubMenu(
	"HelpChildren",
	LOCTEXT("HelpChildren", "Help Children"),
	LOCTEXT("", ""),
	FNewToolMenuChoice()
	);
```




## + UToolMenu 主要函数

确切来说，UToolMenu 只管理 Section，而每个 Section 再管理 Entry。因此 UToolMenu 中的主要函数与管理 Sections 有关。

![](https://pic2.zhimg.com/v2-9ec0b3f8db256627651a19b9a53e5543_r.jpg)

**AddSection()** 是最核心的函数，用于向 Sections 成员变量中添加 Section，反向操作是 **RemoveSection()**。

![](https://pica.zhimg.com/v2-b519397c733691d681abd8f3a444383a_r.jpg)

在添加 Section 时可以指定它在菜单中的位置顺序，由 FToolMenuInsert 表示，其中 Name 为指定其他 Section 的名字，Position 代表插入位置在那个 Section 之前还是之后。




![](https://pic1.zhimg.com/v2-1af361b05a10e818e5839d9872ca56c6_r.jpg)

一个更简便的添加 Section 的函数是 **FindOrAddSection()**，如果对添加 Section 的细节没有要求，或无需对现有 Section 的细节做变更，则可以使用这个函数。




![](https://pic1.zhimg.com/v2-634831118e7bfc3f285210b774452ecc_r.jpg)

**AddDynamicSection()** 允许开发者动态地增改 MenuSection，下面是一个简单的示例。

```cpp
UToolMenu* ExsitingMenu = UToolMenus::Get()->FindMenu("LevelEditor.MainMenu");
FToolMenuSection& ExsitingSection = ExsitingMenu->FindOrAddSection(NAME_None);

FToolMenuEntry& DynamicSectionTest =
ExsitingSection.AddSubMenu(
	"DynamicSectionTest",
	LOCTEXT("DynamicSectionTest", "Dynamic Section Test"),
	LOCTEXT("", ""),
	FNewToolMenuChoice()
	);
DynamicSectionTest.InsertPosition = FToolMenuInsert("Help", EToolMenuInsertType::After);

static const FName DynamicSectionTestName = "LevelEditor.MainMenu.DynamicSectionTest";
UToolMenu* DynamicSectionTestMenu = UToolMenus::Get()->RegisterMenu(DynamicSectionTestName);

DynamicSectionTestMenu->AddDynamicSection("NewSection",
	FNewToolMenuDelegate::CreateLambda([](UToolMenu* InMenu)
	{
		// if(...) 可以在此增加灵活的判断条件
		{
			FToolMenuSection& SubSection = InMenu->AddSection("DynamicSection", LOCTEXT("DynamicSection", "Dynamic Section"));
			SubSection.AddMenuEntry(
				"DynamicSectionTestEntry",
				LOCTEXT("DynamicSectionTestEntry", "Dynamic Section Test Entry"),
				LOCTEXT("", ""),
				FSlateIcon(),
				FUIAction());
			
		}
	}));
```

这段代码演示了通过 AddDynamicSection() 在 DynamicSectionTest 菜单下添加一个名为 DynamicSection 的 Section，并添加名为 DynamicSectionTestEntry 的菜单按钮。

值得注意的是名为 NewSection 的 Section，即真正的 DynamicSection 不会显示。

除了增改 Section，由于传入回调函数中的参数是 UToolMenu*，所以也可以做一些奇怪的操作，譬如删除 Section 或 Entry：

```cpp
UToolMenu* ExsitingMenu = UToolMenus::Get()->FindMenu("LevelEditor.MainMenu");
FToolMenuSection& ExsitingSection = ExsitingMenu->FindOrAddSection(NAME_None);

FToolMenuEntry& DynamicSectionTest =
ExsitingSection.AddSubMenu(
	"DynamicSectionTest",
	LOCTEXT("DynamicSectionTest", "Dynamic Section Test"),
	LOCTEXT("", ""),
	FNewToolMenuChoice()
	);

DynamicSectionTest.InsertPosition = FToolMenuInsert("Help", EToolMenuInsertType::After);
static const FName DynamicSectionTestName = "LevelEditor.MainMenu.DynamicSectionTest";
UToolMenu* DynamicSectionTestMenu = UToolMenus::Get()->RegisterMenu(DynamicSectionTestName);

// 在 LevelEditor.MainMenu.DynamicSectionTest 中增加静态 MenuSection 和 MenuEntry
FToolMenuSection& StaticSection = DynamicSectionTestMenu->FindOrAddSection("StaticSection");
StaticSection.AddMenuEntry(
	"StaticSectionEntry",
	LOCTEXT("StaticSectionEntry", "Static Section Entry"),
	LOCTEXT("", ""),
	FSlateIcon(),
	FUIAction());

DynamicSectionTestMenu->AddDynamicSection("NewSection",
	FNewToolMenuDelegate::CreateLambda([](UToolMenu* InMenu)
	{
		// if(...)
		{
			// 删除静态 MenuSection 和 MenuEntry
			UToolMenus::Get()->RemoveEntry("LevelEditor.MainMenu.DynamicSectionTest", "StaticSection", "StaticSectionEntry");
			InMenu->RemoveSection("StaticSection");
			// 用菜单定制化修正第一次点击无法正确删除 MenuEntry 的问题
			//UToolMenus::Get()->AddMenuCustomization("LevelEditor.MainMenu.DynamicSectionTest")->AddEntry("StaticSectionEntry")->Visibility = ECustomizedToolMenuVisibility::Hidden;
		
			FToolMenuSection& SubSection = InMenu->AddSection("DynamicSection", LOCTEXT("DynamicSection", "Dynamic Section"));
			SubSection.AddMenuEntry(
				"DynamicSectionTestEntry",
				LOCTEXT("DynamicSectionTestEntry", "Dynamic Section Test Entry"),
				LOCTEXT("", ""),
				FSlateIcon(),
				FUIAction());
		}
	}));
```

然而由于构建流程处理顺序的问题，回调函数中的 RemoveEntry() 并不会立即生效，StaticSectionEntry 会在第一次构建 LevelEditor.MainMenu.DynamicSectionTest 时保留，这个问题可以通过菜单定制化来规避。

![](https://picx.zhimg.com/v2-0fcae655470fc13368b8e46542cae89f_r.jpg)







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## Section（FToolMenuSection）

![](https://pica.zhimg.com/v2-82ca9aa2c8ae473c4f1f6bce5b6f0df6_r.jpg)

**FToolMenuSection **表示 UToolMenu 中的 Section，负责管理一个 Section 下的 Entry。Section 中的 Entries 被记录在名为 Blocks 的成员变量中。

![](https://pic1.zhimg.com/v2-c5f1205d33050954c658597608f6be92_r.jpg)

**AddEntry()** 是其中的关键函数，用于向 Blocks 中增加 Entry。反向操作由 **RemoveEntry()** 执行，但 FToolMenuSection 对象无法直接使用 RemoveEntry()，因为它是一个私有函数，要删除 UToolMenu 中的 Entry，只能使用 **UToolMenus::Get()->RemoveEntry()** 来统一调度。




![](https://pic1.zhimg.com/v2-5ddee798abb071513e746e70ba33e676_r.jpg)

**AddDynamicEntry()** 允许开发者动态地增加 Entry，回调函数传入参数为 FToolMenuSection&。




FToolMenuSection 的其他函数主要都负责添加各种不同的 Entry，关于 Entry 的不同类型，我们在接下来的 MenuEntry 中来探讨。







${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




## MenuEntry（FToolMenuEntry）

![](https://pic3.zhimg.com/v2-ef7014a9c58334a722df7f047b201328_r.jpg)

**FToolMenuEntry **是 UToolMenu 中的实际项目，它可以有多种类型存在，从 FToolMenuEntry 中的初始化函数可以看出来，包括 MenuEntry，SubMenu，ToolBarButton，ComboButton，Separator，Widget 一些。

一个 FToolMenuEntry 的类型是由它的名为 **Type **的成员变量记录的，一个 EMultiBlockType 类型。与 FToolMenuSection 类似，InsertPosition 用于确定 Entry 在插入 Section 时相对于其他 Entry 的位置，它是一个公开成员变量，可以直接设置。

FToolMenuEntry 中关键的函数是 **TryExecuteToolUIAction()**，它负责触发 **Action **成员变量中的操作。

```cpp
Section.AddMenuEntry(
	"SomeEntry",
	LOCTEXT("SomeEntry", "Some Entry"),
	LOCTEXT("", ""),
	FSlateIcon(),
	FUIAction() //提供Entry操作
);
```

**Action **一般由添加 Entry 时通过参数提供，但也有例外的情况：

![](https://picx.zhimg.com/v2-3b091b4b02a2be82a198280fb10e4521_r.jpg)

在 FPopulateMenuBuilderWithToolMenuEntry::AddStandardEntryToMenuBuilder() 中，我们可以根据代码的逻辑推理 FToolMenuEntry 中的命令优先级：

**FToolMenuEntry::Command > FToolMenuEntry::ScriptObject > FToolMenuEntry::Action**

**Command** 是最优先的，通过 Command 可以为命令设置快捷键，从而将菜单项与快捷键关联起来。
**ScriptObject **提供了从蓝图设置菜单操作的能力。
**Action **则是提供普通的菜单操作。

在将 Entry 填充到 Builder 中时（当 Entry 类型为一个普通 MenuEntry 时），会根据不同的优先级调用对应的 AddMenuEntry() 函数。因此，下面的情况下 Command 会覆盖 Action（实际上是不会发生这种情况的，因为 **SetCommand()** 是私有函数，我们能对 FToolMenuEntry 做出的修改十分有限）：

```cpp
FToolMenuEntry& MenuEntry = 
Section.AddMenuEntry(
	"SomeEntry",
	LOCTEXT("SomeEntry", "Some Entry"),
	LOCTEXT("", ""),
	FSlateIcon(),
	FUIAction::CreateLambda([](){//...})
);

MenuEntry.SetCommand(...)
```










${♡☘♡☘♡  \quad今天的捉弄结束了 \quad ♡☘♡☘♡}\\$ 




# Builder

![](https://picx.zhimg.com/v2-e2dca49c0a9a279f19d492f7801ac4b7_r.jpg)

**Builder **用于将 UToolMenu 生成为一个 SWidget，根据 UToolMenu 的菜单类型不同，Builder 有很多不同的种类。

Builder 的关键成员变量有两个，**MuiltBox **是用于构建 SMultiBoxWidget 的数据结构，SMultiBoxWidget 是我们最终要生成的 SWidget 类型，在转换 UToolMenu 时，会将每个 Entry 转换为对应的 MuiltBox 元素。**ExtenderStack** 是一个 FExtender 数组，包含所有用于扩展的 Extensions，一个 Extension 代表一次扩展操作，这些 Extensions 储存在一个 FExtender 中，每个 FExtender 相当于一个额外的扩展包，**FExtender** 为 Builder 带来了在生成时为菜单动态添加额外项目的能力。

一句话总结从 UToolMenu 生成 SWidget 的过程，这个流程由 UToolMenus 发起，UToolMenus 收集并组织要生成 SWidget 的 UToolMenu 结构，并将 UToolMenu 填充到 Builder 的 MuiltBox，在这个填充的过程中，Builder 会将 FExtender 的额外菜单项也一并添加到 MuiltBox，随后，由 MuiltBox 生成 SMultiBoxWidget。

在整个过程中，将 UToolMenu 填充到 Builder 的 MuiltBox 是关键，此时 UToolMenu 的结构已经确定了下来，我们要探索的是 FExtender 是如何被引入进来的。




## UToolMenus 体系中 Builder 的运作原理

![](https://picx.zhimg.com/v2-ad8057d428e345518db59853c0bbdf3b_r.jpg)

不同类型的 UToolMenu 和 Entry 有不同的 Builder 和不同的填充方式，我们以 UToolMenu 类型为 **EMultiBoxType::Menu**，Entry 类型为 **EMultiBlockType::MenuEntry **的情况为例，此时将使用 **FBaseMenuBuilder::AddMenuEntry()** 来将 Entry 填充到 MuiltBox 中。




![](https://picx.zhimg.com/v2-7710885416bbbbdad2adb6454059ad09_r.jpg)

在构建每一个 UToolMenu 中的 MenuEntry 时，Builder 将传入 MenuEntry 的名字作为 ExtensionHook，通过 **FBaseMenuBuilder::ApplyHook()**，Builder 持 ExtensionHook 逐一询问 FExtender 中的 Extension 是否需求相同名称的 Hook，这个操作在正式添加 MenuEntry 的前后各一次，以此确定在 MenuEntry 的前后是否有需要执行的 Extension 操作，如果有某个 Extension 需求的 ExtensionType、Hook、HookPosition 与此时的条件完全一致，则执行这个 Extension 的操作。

这里 Extension 中的 Hook 是在向 FExtender 中添加 Extension 时设置的，例如下面这样，此时为 Extension 设置了它要挂靠的 Hook：

```cpp
MenuExtender->AddMenuExtension(
	"Copy", // 、Hook Name
	EExtensionHook::Before,
	nullptr,
	FMenuExtensionDelegate::CreateLambda([](){ //... })
);
```

这里的巧妙之处在于 Extension 触发的操作会重复 FBaseMenuBuilder::AddMenuEntry()（只要它想通过 Builder 中的Add函数添加额外的菜单项），直到遍历 FExtender 中所有的 Extensions 不再有符合条件的 Extension。这样循环遍历的好处是即使通过 FExtender 添加的菜单项目，也可以具有自己的 ExtensionHook，并且也会被下一次 Add 过程考量到，例如下面这样，为 Extension 操作中添加的 Entry 自己的 ExtensionHook：

```cpp
// 某个Extension 代理中的添加函数
MenuBuilder.AddMenuEntry(
	LOCTEXT("MyAddedEntry", "My Added Entry"),
	LOCTEXT("", ""),
	FSlateIcon(),
	FUIAction(FExecuteAction::CreateLambda([](){ //... })),
	"EntryHook" // 这个Entry自己的ExtensionHook
);
```




## FExtender，FExtensibilityManager，FToolMenuContext 与 Builder

如何将 FExtender 添加到 Builder 中，将成为 FExtender 是否生效的关键。一个很好的示例在 FlevelEditorMenu 中。

**FExtensibilityManager** 是管理 Fextender 的工具，例如，为了在 LevelEditor 中添加一个额外的菜单项，需要使用到下面的代码：

```cpp
TSharedPtr<FExtender> MainMenuExtender = MakeShareable(new FExtender);

MainMenuExtender->AddMenuExtension(
	"EpicGamesHelp",
	EExtensionHook::After,
	nullptr,
	FMenuExtensionDelegate::CreateLambda([](){ //... })
	);
	
FLevelEditorModule& LevelEditorModule = 
FModuleManager::Get().LoadModuleChecked<FLevelEditorModule>("LevelEditor");

TSharedPtr<FExtensibilityManager> MenuExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();

MenuExtensibilityManager->AddExtender(MainMenuExtender);
```

我们直接将 MainMenuExtender 添加到 MenuExtensibilityManager 这个 FExtensibilityManager 中，为什么就能够让 FExtender 起作用了呢？




![](https://pic1.zhimg.com/v2-00dcc8ca50fa2e131aa83e81ddc504fc_r.jpg)

**FExtensibilityManager **能够收集 FExtender，作为一个数组储存，并在需要的时候使用 FExtensibilityManager::GetAllExtenders() 将这些 FExtenders 合并为一个 FExtender 输出。

![](https://pic2.zhimg.com/v2-f79ee04cf3603d51a1f95dff8fca3d1d_r.jpg)

在 **FToolMenuContext **中，有一个专门储存 FExtender 的成员变量，FExtender 借助 FToolMenuContext 在 UToolMenus 流程中传递，最终传入创建的 UToolMenu 中，又在 Builder 创建时被记录到 Context 成员变量。




所以，当我们需要为自己的模块或工具添加 FExtender 扩展功能时，也可以借助 FExtensibilityManager：

```cpp
// 1. 添加 FExtensibilityManager
SomeClass
{
public:
	FExtensibilityManager MyExtensibilityManager;
}

// 2. 将 FExtensibilityManager 中的 FExtender 添加到 FToolMenuContext，并交给 GenerateWidget()
TSharedPtr<FExtender> Extenders = SomeClass.MyExtensibilityManager->GetAllExtenders();
FToolMenuContext ToolMenuContext(CommandList, Extenders.ToSharedRef());
TSharedRef<SWidget> MenuWidget = UToolMenus::Get()->GenerateWidget(MenuName, ToolMenuContext);
```




## 单独使用 Builder

Builder 作为菜单控件的“构建器”参与到 UToolMenus 体系的一部分，但 Builder 本身的用途使它也可以被单独使用。我们不妨详细来整理一下 Builder 自身的运作原理。

![](https://pica.zhimg.com/v2-1748e659a2878f8cc9b8ebd0d8ae6674_r.jpg)

我们已经提到，Builder 中有一个储存要构建菜单控件结构的容器 MuiltBox，它是一个 **FMultiBox **共享引用类型的成员变量。FMultiBox 继承自 FMultiBlock，其中有一个 FMultiBox 共享引用类型的数组，这让我们得以以数组的形式来储存菜单控件结构。

对于继承自 FMultiBoxBuilder 的其他 Builder，提供了各种 Add 方法来向 MuiltBox 中添加自己的内容，以 FMenuBuilder 举例，有下面这些：

```cpp
FBaseMenuBuilder::AddMenuEntry()
FMenuBuilder::AddMenuSeparator()
FMenuBuilder::AddSeparator()
FMenuBuilder::AddSubMenu()
FMenuBuilder::AddWrapperSubMenu()
FMenuBuilder::AddWidget()
FMenuBuilder::AddSearchWidget()
```

由于 MuiltBox 中数组的排序本身就是一个令人头疼的问题，它们需要遵循一套约定俗成的定式，才能完成好这个任务，这些 Add 方法内部大多长得很像：

```cpp
void FBaseMenuBuilder::AddMenuEntry( const TSharedPtr< const FUICommandInfo > InCommand, FName InExtensionHook, const TAttribute<FText>& InLabelOverride, const TAttribute<FText>& InToolTipOverride, const FSlateIcon& InIconOverride, FName InTutorialHighlightName )
{
	ApplySectionBeginning();

	ApplyHook(InExtensionHook, EExtensionHook::Before);	// step 1
	
	// The command must be valid
	check( InCommand.IsValid() );
	TSharedRef< FMenuEntryBlock > NewMenuEntryBlock = MakeShared<FMenuEntryBlock>( InExtensionHook, InCommand, CommandListStack.Last(), InLabelOverride, InToolTipOverride, InIconOverride, bCloseSelfOnly );
	NewMenuEntryBlock->SetTutorialHighlightName(GenerateTutorialIdentfierName(TutorialHighlightName, InTutorialHighlightName, InCommand, MultiBox->GetBlocks().Num()));
	MultiBox->AddMultiBlock( NewMenuEntryBlock );		// step 2

	ApplyHook(InExtensionHook, EExtensionHook::After);	// step 3
}
```

具体的规则是，当需要向数组中添加内容时：

1. 先询问在这个内容之前是否有需要添加的内容，如果有，就添加，并继续询问，递归这个添加流程，直到没有更多需要添加的内容。
2. 添加当前要添加的内容。
3. 再询问在这个内容之后是否有需要添加的内容，如果有，就添加，并继续询问，递归这个添加流程，直到没有更多需要添加的内容。

如果要创建自己的 Builder，也需要遵循这样的方式来创建自己的函数。




说回 Add 方法本身，例如 AddMenuEntry() 这种十分通用的函数，它支持多种不同的创建来源：

```cpp
// FBaseMenuBuilder
void AddMenuEntry( const TSharedPtr< const FUICommandInfo > InCommand, FName InExtensionHook = NAME_None, const TAttribute<FText>& InLabelOverride = TAttribute<FText>(), const TAttribute<FText>& InToolTipOverride = TAttribute<FText>(), const FSlateIcon& InIconOverride = FSlateIcon(), FName InTutorialHighlightName = NAME_None );
void AddMenuEntry( const TAttribute<FText>& InLabel, const TAttribute<FText>& InToolTip, const FSlateIcon& InIcon, const FUIAction& UIAction, FName InExtensionHook = NAME_None, const EUserInterfaceActionType UserInterfaceActionType = EUserInterfaceActionType::Button, FName InTutorialHighlightName = NAME_None );
void AddMenuEntry( const FUIAction& UIAction, const TSharedRef< SWidget > Contents, const FName& InExtensionHook = NAME_None, const TAttribute<FText>& InToolTip = TAttribute<FText>(), const EUserInterfaceActionType UserInterfaceActionType = EUserInterfaceActionType::Button, FName InTutorialHighlightName = NAME_None );
void AddMenuEntry( const FMenuEntryParams& InMenuEntryParams );
```

其中带有 `TSharedRef< SWidget > Contents` 这种甚至可以以自己的控件来替换原有统一的内容形式。如何做到的？

![](https://pica.zhimg.com/v2-a8b947d14258c906960d41b8c5f2f320_r.jpg)

通过不同的 FMenuEntryBlock 构造函数，我们可以向 FMenuEntryBlock 中传递不同的内容，再由 FMenuEntryBlock 对应的 SMenuEntryBlock 根据这些内容，处理和生成相应的控件。我们在这里也可以看到不在 UToolMenus 体系中的 F***Block 和 S***Block。

因此，当想要生成自己的菜单控件时，也可以直接这样做：

```cpp
// Example from FLevelEditorToolBar::RegisterLevelEditorToolBar()


// new Builder
FMenuBuilder MenuBuilder(true, MakeShareable(new FUICommandList));

// 准备UICommandList, 压入UICommandList
TSharedRef<FUICommandList> SomeCommandList = GetSomeCommandList();
MenuBuilder.PushCommandList(SomeCommandList);

// 准备UICommandInfo
TArray<TSharedPtr<FUICommandInfo>> CommandInfos = GetCommandInfos();

// Add MenuEntry
MenuBuilder.BeginSection("SomeSection");
for(TSharedPtr<FUICommandInfo> Command : CommandInfos)
{
	MenuBuilder.AddMenuEntry(Command);
}
MenuBuilder.EndSection();

// 生成 SWidget
MenuBuilder.MakeWidget();

// 弹出UICommandList
MenuBuilder.PopCommandList();
```







![](https://pic4.zhimg.com/v2-ac116271a1b1fd849180954be47f3c99_r.jpg)

