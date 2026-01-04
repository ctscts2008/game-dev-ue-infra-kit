#include "CustomSlate.h"

#include "ToolMenus.h"
#include "ExtendEditorStyle.h"

#include "Widgets/Layout/SConstraintCanvas.h"


IMPLEMENT_MODULE(FCustomSlateModule, CustomSlate);

void FCustomSlateModule::StartupModule()
{
	IModuleInterface::StartupModule();

	CreateButtonByStyle();
}
void FCustomSlateModule::ShutdownModule()
{
IModuleInterface::ShutdownModule();

	// 在模块注销时注销
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FName("MainLayoutTab"));
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FName("SubTab.1"));
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FName("SubTab.2"));
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
	//SpawnWindow();

		//SpawnTab();

	SpawnLayoutTab();
}

void FCustomSlateModule::SpawnWindow()
{
	TSharedRef<SWindow> NewWindow = SNew(SWindow)
		.Title(FText::FromString("Custom Slate Window Title"))
		.SizingRule(ESizingRule::FixedSize)
		.ClientSize(FVector2D(960.f, 720.f))
		.AutoCenter(EAutoCenter::PrimaryWorkArea)
		.ScreenPosition(FVector2D(100,0))
		.IsEnabled(true)
		.SupportsMaximize(false)
		.SupportsMinimize(false);
		
		NewWindow->SetContent(
			SNew(SConstraintCanvas)
			+ SConstraintCanvas::Slot()
			.Alignment(0.5f)
			.Anchors(0.5f)
			.Offset(FMargin(0.f, 0.f, 300.f, 100.f))
			.AutoSize(true)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Custom Slate Window"))
				
			]);

	FSlateApplication::Get().AddWindow(NewWindow);
}

void FCustomSlateModule::SpawnTab()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("My Tab"),
		FOnSpawnTab::CreateLambda([](const FSpawnTabArgs& SpawnArgs )->TSharedRef<SDockTab> 
		{
			TSharedRef<SDockTab> Tab = SNew(SDockTab).TabRole(DocumentTab);

			Tab->SetContent
			(
				SNew(SConstraintCanvas)
				+SConstraintCanvas::Slot()
				.Alignment(0.5f)
				.Anchors(0.5f)
				.Offset(FMargin(0.f, 0.f, 300.f, 100.f))
				.AutoSize(true)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Tab Slot Text"))
				]
			);
           return Tab;
		})).SetDisplayName(FText::FromString("My Tab"));

	FGlobalTabmanager::Get()->TryInvokeTab(FName("My Tab"));
}

TSharedRef<SDockTab> FCustomSlateModule::OnSpawnMainTab(const FSpawnTabArgs& SpawnArgs)
{
	TSharedRef<SDockTab> MainTab = SNew(SDockTab).TabRole(ETabRole::NomadTab);
	if (!TabManager.IsValid())
	{
		TabManager = FGlobalTabmanager::Get()->NewTabManager(MainTab);
	}
	if (!TabMananagerLayOut.IsValid())
	{
		TabMananagerLayOut = FTabManager::NewLayout("MyTabManagerMainLayout")
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
			->Split(
				FTabManager::NewStack()
							->SetSizeCoefficient(.4f)
							->AddTab("SubTab.3", ETabState::OpenedTab)
							->SetHideTabWell(true)
			)
			
				);
		
	}
	TSharedRef<SWidget> TabContents = TabManager->RestoreFrom(
TabMananagerLayOut.ToSharedRef(), TSharedPtr<SWindow>()).ToSharedRef();

	MainTab->SetContent
	(
		TabContents
	);

	return MainTab;		
}

TSharedRef<SDockTab> FCustomSlateModule::OnSpawnSubTab(const FSpawnTabArgs& SpawnArg)
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
			.Text(FText::FromString("Custom Slate SubTab"))
		]
	);

	return Tab;
}

void FCustomSlateModule::SpawnLayoutTab()
{
		FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("MainLayoutTab"),
			FOnSpawnTab::CreateRaw(this, &FCustomSlateModule::OnSpawnMainTab)
			).SetDisplayName(FText::FromString("Main Layout Tab"));
	
			FGlobalTabmanager::Get()->RegisterNomadTabSpawner( FName("SubTab.1"),
			FOnSpawnTab::CreateRaw( this, &FCustomSlateModule::OnSpawnSubTab)
			).SetDisplayName(FText::FromString("SubTab 1"));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("SubTab.2"),
	FOnSpawnTab::CreateRaw( this, &FCustomSlateModule::OnSpawnSubTab)
	).SetDisplayName(FText::FromString("SubTab 2"));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("SubTab.3"),
FOnSpawnTab::CreateRaw( this, &FCustomSlateModule::OnSpawnSubTab)
).SetDisplayName(FText::FromString("SubTab 3"));

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
