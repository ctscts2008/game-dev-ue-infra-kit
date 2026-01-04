#include "CustomNormalDistributionEditorToolKit.h"
#include "Widgets/Docking/SDockTab.h"
#include "SCustomNormalDistributionWidget.h"
#include "Modules/ModuleManager.h"

void FCustomNormalDistributionEditorToolKit::InitEditor(const TArray<UObject*>& InObjects)
{
	if (InObjects.Num() <= 0)
	{
		return;
	}
	NormalDistribution = Cast<UCustomNormalDistribution>(InObjects[0]);

	const TSharedRef<FTabManager::FLayout> layout =
		FTabManager::NewLayout("CustomNormalDistributionEditorTools.layout")
	->AddArea(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
		->Split(
					FTabManager::NewSplitter()
					->SetSizeCoefficient(0.6f)
					->SetOrientation(Orient_Horizontal)
					->Split
					(
								FTabManager::NewStack()
								->SetSizeCoefficient(0.8f)
								->AddTab("CustomNormalDistributionDetailsTab", ETabState::OpenedTab)
					)
					->Split
					(
							FTabManager::NewStack()
							->SetSizeCoefficient(0.2f)
							->AddTab("CustomNormalDistributionDetailsTab", ETabState::OpenedTab)

					)
					->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.4f)
				->AddTab("OutputLog", ETabState::OpenedTab)
			)
		
		));
	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, {}, "CustomNormalDistributionEditor", layout, true, true, InObjects);
	
}
void FCustomNormalDistributionEditorToolKit::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	WorkspaceMenuCategory = 
		InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("CustomNormalDistributionTabs"));

	// 注册 SCustomNormalDistributionWidget TabSpawner
	InTabManager->RegisterTabSpawner("CustomNormalDistributionPDFTab", 
		FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs&)
		{
			return SNew(SDockTab)
			[
				SNew(SCustomNormalDistributionWidget)
				.Mean(this, &FCustomNormalDistributionEditorToolKit::GetMean)
				.StandardDeviation(this, &FCustomNormalDistributionEditorToolKit::GetStandardDeviation)
				.OnMeanChanged(this, &FCustomNormalDistributionEditorToolKit::SetMean)
				.OnStandardDeviationChanged(this, &FCustomNormalDistributionEditorToolKit::SetStandardDeviation)
			];
		}))
	.SetDisplayName(INVTEXT("PDF"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	// 创建 CustomNormalDistribution DetailsView
	FPropertyEditorModule& PropertyEditorModule = 
		FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	TSharedRef<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObjects(TArray<UObject*>{ NormalDistribution });

	// 注册 CustomNormalDistribution DetailsView TabSpawner
	InTabManager->RegisterTabSpawner("CustomNormalDistributionDetailsTab", 
		FOnSpawnTab::CreateLambda([=](const FSpawnTabArgs&)
		{
			return SNew(SDockTab)
			[
				DetailsView
			];
		}))
	.SetDisplayName(INVTEXT("Details"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FCustomNormalDistributionEditorToolKit::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner("CustomNormalDistributionPDFTab");
	InTabManager->UnregisterTabSpawner("CustomNormalDistributionDetailsTab");
}

float FCustomNormalDistributionEditorToolKit::GetMean() const
{
	return NormalDistribution->Mean;
}

float FCustomNormalDistributionEditorToolKit::GetStandardDeviation() const
{
	return NormalDistribution->StandardDeviation;
}

void FCustomNormalDistributionEditorToolKit::SetMean(float Mean)
{
	NormalDistribution->Modify();
	NormalDistribution->Mean = Mean;
}

void FCustomNormalDistributionEditorToolKit::SetStandardDeviation(float StandardDeviation)
{
	NormalDistribution->Modify();
	NormalDistribution->StandardDeviation = StandardDeviation;
}