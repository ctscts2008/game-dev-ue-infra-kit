#include "CustomNormalDistributionActions.h"
#include "CustomNormalDistribution.h"
#include "CustomNormalDistributionEditorToolKit.h"
#include "Toolkits/IToolkitHost.h"

FCustomNormalDistributionActions::FCustomNormalDistributionActions(EAssetTypeCategories::Type InAssetCategory)
	:AssetCategory(InAssetCategory){
	
}

UClass* FCustomNormalDistributionActions::GetSupportedClass() const
{
	return UCustomNormalDistribution::StaticClass();
}

FText FCustomNormalDistributionActions::GetName() const
{
	return INVTEXT("Custom Normal Distribution");
}

FColor FCustomNormalDistributionActions::GetTypeColor() const
{
	return FColor::Orange;
}

uint32 FCustomNormalDistributionActions::GetCategories()
{
	return AssetCategory;
}

void FCustomNormalDistributionActions::GetActions(const TArray<UObject*>& InObjects,FMenuBuilder& MenuBuilder)
{
	FAssetTypeActions_Base::GetActions(InObjects, MenuBuilder);


	MenuBuilder.AddMenuEntry(
		FText::FromString("Custom Action"),
		FText::FromString("This is a Custom Action"),	
		FSlateIcon(),
		FUIAction());
}

bool FCustomNormalDistributionActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return true;
}

void FCustomNormalDistributionActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	MakeShared<FCustomNormalDistributionEditorToolKit>()->InitEditor(InObjects);
}





