#include "ExtendMenuBase.h"


#include "MessageClass.h"


#include "LevelEditor.h"
#include "ContentBrowserModule.h"


#include "ExtendEditorAction.h"
#include "ExtendEditorStyle.h"
#include "Dialog/SCustomDialog.h"

#include "Misc/MessageDialog.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"



IMPLEMENT_GAME_MODULE(FExtendMenuBase, ExtendMenuBase)

void FExtendMenuBase::StartupModule()
{
	IModuleInterface::StartupModule();

	// CreateMenuBar();
	//
	// ExcuteContentBrowserExtend();
	//
	// //ExtendByToolMenus();
	//
	// FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	// LevelEditorModule.OnLevelEditorCreated().AddRaw(this, &FExtendMenuBase::OnLevelEditorCreatedEvent);

	//StyleSample();

	//CommandSample();
}

void FExtendMenuBase::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
int32 FExtendMenuBase::ClickCount = 0;
void FExtendMenuBase::StyleButtonAction()
{
	// UE_LOG(LogTemp, Warning, TEXT("StyleButtonAction is called"))
	// if(!MessageDelegate)
	// {
	// 	MessageDelegate = NewObject<UMessageClass>();
	// 	MessageDelegate->AddToRoot();
	// 	MessageDelegate->SetMessage(
	// 	FText::FromString("This is a Binded Message."), FLinearColor::White);
	//
	// 	if(!MessageDelegate->IsBind())
	// 	{
	// 		MessageDelegate->BindToVpStatsSubsystem();
	// 	}
	// }
	//
	// MessageDelegate->ToggleDisplay();


	//MessageDialog
	// FText Title = FText::FromString("EAppMsgType::Ok");
	// EAppReturnType::Type Return = FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("This is a Ok Dialog"), &Title);
	//
	// if (Return == EAppReturnType::Ok)
	// {
	// 	Title = FText::FromString("EAppMsgType::YesNo");
	// 	Return = FMessageDialog::Open(
	// 		EAppMsgType::YesNo, 
	// 		FText::FromString("This is a YesNo Dialog"), 
	// 		&Title
	// 		);
	// }
	// if(Return == EAppReturnType::Yes)
	// {
	// 	Title = FText::FromString("EAppMsgType::OkCancel");
	// 	Return = FMessageDialog::Open(
	// 		EAppMsgType::OkCancel, 
	// 		FText::FromString("This is a OkCancel Dialog"), 
	// 		&Title
	// 		);
	// }
	// if(Return == EAppReturnType::Ok)
	// {
	// 	Title = FText::FromString("EAppMsgType::YesNoCancel");
	// 	Return = FMessageDialog::Open(
	// 		EAppMsgType::YesNoCancel,
	// 		FText::FromString("This is a YesNoCancel Dialog"), 
	// 		&Title
	// 		);
	// }
	// if(Return == EAppReturnType::Yes)
	// {
	// 	Title = FText::FromString("EAppMsgType::CancelRetryContinue");
	// 	Return = FMessageDialog::Open(
	// 		EAppMsgType::CancelRetryContinue, 
	// 		FText::FromString("This is a CancelRetryContinue Dialog"), 
	// 		&Title
	// 		);
	// }
	// if(Return == EAppReturnType::Continue)
	// {
	// 	Title = FText::FromString("EAppMsgType::YesNoYesAllNoAll");
	// 	Return = FMessageDialog::Open(
	// 		EAppMsgType::YesNoYesAllNoAll, 
	// 		FText::FromString("This is a YesNoYesAllNoAll Dialog"), 
	// 		&Title
	// 		);
	// }
	// if(Return == EAppReturnType::Yes || Return == EAppReturnType::YesAll)
	// {
	// 	Title = FText::FromString("EAppMsgType::YesNoYesAllNoAllCancel");
	// 	Return = FMessageDialog::Open(
	// 		EAppMsgType::YesNoYesAllNoAllCancel, 
	// 		FText::FromString("This is a YesNoYesAllNoAllCancel Dialog"), 
	// 		&Title
	// 		);
	// }
	// if(Return == EAppReturnType::Yes || Return == EAppReturnType::YesAll)
	// {
	// 	Title = FText::FromString("EAppMsgType::YesNoYesAll");
	// 	Return = FMessageDialog::Open(
	// 		EAppMsgType::YesNoYesAll, 
	// 		FText::FromString("This is a YesNoYesAll Dialog"), 
	// 		&Title
	// 		);
	// }

	//CustomDialog
// #define  LOCTEXT_NAMESPACE "SCustomDialog"
// TSharedRef<SCustomDialog> NewDialog = SNew(SCustomDialog)
// 	.Title(FText(LOCTEXT("NewSample", "Hello World!")))
// 	.DialogContent(
// 		SNew(STextBlock).Text(FText::FromString("This is a Custom dialog"))).IconBrush("AppIcon")
// 	.Buttons({
// 		SCustomDialog::FButton(LOCTEXT("OK", "OK")),
// 		SCustomDialog::FButton(LOCTEXT("Cancel", "Cancel")),
// 	});
// 	NewDialog->ShowModal();
// #undef LOCTEXT_NAMESPACE

	//Notify
	++ClickCount;
	FString str = TEXT("呀~ 被戳了 ") + FString::FromInt(ClickCount) + TEXT(" 下！~~❤");
	FText Message = FText::FromString(str);

	// 构造 FNotificationInfo
	FNotificationInfo NotifyInfo(Message);
	// 使用大字体
	NotifyInfo.bUseLargeFont = true;
	// 设定通知显示时长
	NotifyInfo.FadeOutDuration = 7.f;
	// 设定通知 Icon
	if (auto style = FExtendEditorStyle::GetStyleSet())
	{
		NotifyInfo.Image = style->GetBrush("AliceTool") ;
	}

	// 利用 FNotificationInfo 发送通知
	FSlateNotificationManager::Get().AddNotification(NotifyInfo);
}



struct  Local
{
public:
	static  void ExistingMenuEntryAction()
	{
		UE_LOG(LogTemp, Warning, TEXT("ExistingMenuEntryAction is called"))
	}
	static  void ExistingMenuBarEntryAction()
	{
		UE_LOG(LogTemp, Warning, TEXT("ExistingMenuBarEntryAction is called"))
	}
	static void MakeExistingMainMenuEntry(FMenuBuilder& MenuBuilder)
	{
		MenuBuilder.AddMenuEntry(
FText::FromString(TEXT("Existing Hook Button")),
FText::FromString(TEXT("This is a existing hook extend")),
FSlateIcon(),
FUIAction(FExecuteAction::CreateStatic(&Local::ExistingMenuEntryAction))
);
	}

	struct localnewmenu
	{
	public:
		static  void ExistingMenuButtonAction()
		{
			UE_LOG(LogTemp, Warning, TEXT("ExistingMenuButtonAction is called"))
		}

		static  void ExistingToolBarButtonAction()
		{
			UE_LOG(LogTemp, Warning, TEXT("ExistingToolBarButtonAction is called"))
			
		}
		static void MakeNewmenuentry(FMenuBuilder& MenuBuilder)
		{
			MenuBuilder.BeginSection("Section 1", FText::FromString(TEXT("New Menu 1")));
			MenuBuilder.AddMenuEntry(
				FText::FromString(TEXT("New Menu Button")),
				FText::FromString(TEXT("This is a new Menu btn")),
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateStatic(&localnewmenu::ExistingMenuButtonAction))
				);
			MenuBuilder.EndSection();
		}
	};

	static void MakeExistingMainMenuBarEntry(FMenuBarBuilder& MenuBarBuilder)
	{
		MenuBarBuilder.AddPullDownMenu(
			FText::FromString(TEXT("Existing Menu Bar")),
			FText::FromString(TEXT("This is a new menu")),
			FNewMenuDelegate::CreateStatic(&localnewmenu::MakeNewmenuentry),
			"new menu hook");
	}
	
	static void MakeExistingMainMenuToolBarEntry(FToolBarBuilder& ToolBarBuilder)
	{
		ToolBarBuilder.AddSeparator("TestSp");
		ToolBarBuilder.AddToolBarButton(FUIAction(FExecuteAction::CreateStatic(&localnewmenu::ExistingToolBarButtonAction)));
	
	}
};

struct BrowsercontentLocalDec
{
public:
	static  void ExistingAssetButtonAction()
	{
		UE_LOG(LogTemp, Warning, TEXT("ExistingAssetButtonAction is called"))
	}
	
	void static MakeAssetViewContextMenuEntry(FMenuBuilder& MenuBuilder)
	{
		MenuBuilder.EndSection();// 可能引擎某个地方忘了加endsection了  这边1不重置name会命中check test version：5.7
		MenuBuilder.BeginSection("Asset Section 1", FText::FromString(TEXT("Asset Section 1")));

		MenuBuilder.AddMenuEntry(
			FText::FromString(TEXT("Asset zview Context Menu Button")),
			FText::FromString(TEXT("This is a Asset zview Context Menu Button")),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateStatic(&BrowsercontentLocalDec::ExistingAssetButtonAction)));

		MenuBuilder.EndSection();
	}

	

};
#define LOCTEXT_NAMESPACE "ExtendByToolMenus"
/**
 * some cases for Extend by tool menus
 */
void FExtendMenuBase::ExtendByToolMenus()
{
	UToolMenu* Menu = UToolMenus::Get()->FindMenu("LevelEditor.MainMenu");

	FToolMenuSection& Section = Menu->FindOrAddSection(NAME_None);

	FToolMenuEntry& MakeEntry =
		Section.AddSubMenu(
			"NewMenuByUToolMenus",
			LOCTEXT("NewMenu2", "New Menu 2"),      // 标签
			LOCTEXT("NewMenu2_ToolTip", "This is a extended menu by UToolMenus"), // ToolTip
			FNewToolMenuChoice() 

		);

	MakeEntry.InsertPosition = FToolMenuInsert("Help", EToolMenuInsertType::After);

	static const FName BaseMenuName = "LevelEditor.MainMenu.NewMenuByUToolMenus";//注意这边的Name的子节点为想要插入的menuName,这里是
	//上面注册的submenu的Name->NewMenuByUToolMenus 类似Tag的索引关系
	Menu =  UToolMenus::Get()->RegisterMenu(BaseMenuName);

	FToolMenuSection& NewSection = Menu->AddSection("New Section2");//AddSection(
	NewSection.AddMenuEntry(
		"New menu 2 btn",
		FText::FromString("Label: New Menu 2 Button"),
		FText::FromString("This is a new menu 2 button by UtoolMenus"),
		FSlateIcon(),
		FToolUIActionChoice(FExecuteAction::CreateRaw(this, &FExtendMenuBase::NewMenu2ButtonAction)));
	
}

#undef LOCTEXT_NAMESPACE

void FExtendMenuBase::NewMenu2ButtonAction()
{
	UE_LOG(LogTemp, Warning, TEXT("NewMenu2ButtonAction is called."))
}
/*
 *通过FExtend方式拓展contentbrowser
 */
void FExtendMenuBase::ExcuteContentBrowserExtend()
{
	FContentBrowserModule& contentbrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	TArray<FContentBrowserMenuExtender_SelectedPaths>& Extenders = contentbrowserModule.GetAllAssetContextMenuExtenders();

	Extenders.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FExtendMenuBase::ExtendAssetContextMenu));
}

TSharedRef<FExtender> FExtendMenuBase::ExtendAssetContextMenu(const TArray<FString>& SelectedPaths)
{
	TSharedRef<FExtender> MenuExtender(new FExtender());

	if (SelectedPaths.Num() > 0)
	{
		MenuExtender->AddMenuExtension(
			"ImportAsset",
			EExtensionHook::Before,
			TSharedPtr<FUICommandList>(),
			FMenuExtensionDelegate::CreateStatic(&BrowsercontentLocalDec::MakeAssetViewContextMenuEntry));
	}

	return MenuExtender;
}
/*通过FExtend的方式拓展editor
 Sample code**/
void FExtendMenuBase::CreateMenuBar()
{
	TSharedPtr<FExtender> MainMenuExtender = MakeShareable(new FExtender);

	//主菜单下的子按钮
	MainMenuExtender->AddMenuExtension(
		"EpicGamesHelp",
		EExtensionHook::After,
		nullptr,
		FMenuExtensionDelegate::CreateStatic( &Local::MakeExistingMainMenuEntry));

	//主菜单按钮
	MainMenuExtender ->AddMenuBarExtension(
		"Tools",
		EExtensionHook::After,
		nullptr,
		FMenuBarExtensionDelegate::CreateStatic(&Local::MakeExistingMainMenuBarEntry)
		);

	//toolbar 按钮 不生效
	// MainMenuExtender->AddToolBarExtension(
	// "EditorModes",
	// EExtensionHook::After,
	// nullptr,
	// FToolBarExtensionDelegate::CreateStatic(&Local::MakeExistingMainMenuToolBarEntry)
	// );

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
    if (TSharedPtr<FExtensibilityManager> MenuExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager())
    {
    	MenuExtensibilityManager->AddExtender(MainMenuExtender);
    }
}

void FExtendMenuBase::OnLevelEditorCreatedEvent(TSharedPtr<class ILevelEditor> Editor)
{
	ExtendByToolMenus();
}

void FExtendMenuBase::CommandSample()
{
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.User");
	FToolMenuSection& Section = Menu->FindOrAddSection(NAME_None);

	FToolMenuEntry ToolMenuEntry = FToolMenuEntry::InitToolBarButton(FExtendEditorCommands::Get().PrintLog);
	ToolMenuEntry.SetCommandList(FExtendEditorCommands::Get().CommandList);

	Section.AddEntry(ToolMenuEntry);
}

void FExtendMenuBase::StyleSample()
{
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.User");

	FToolMenuSection& Section = Menu->FindOrAddSection(NAME_None);
	Section.AddEntry(FToolMenuEntry::InitToolBarButton(
		"StyleButton",
FToolUIActionChoice(FExecuteAction::CreateRaw(this, &FExtendMenuBase::StyleButtonAction)),
FText::FromString("Label: Style Buttom"),
	FText::FromString("This is a style button by Style Sample"),
	FSlateIcon(FExtendEditorStyle::GetStyleSetName(), "AliceTool")
		)
		);
}


