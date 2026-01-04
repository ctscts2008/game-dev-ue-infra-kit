#include "CustomDataTypeEditor.h"

#include "AssetToolsModule.h"
#include "AssetTypeCategories.h"

IMPLEMENT_MODULE(FCustomDataTypeEditorModule, CustomDataTypeEditor);

void FCustomDataTypeEditorModule::StartupModule()
{
	IModuleInterface::StartupModule();

	EAssetTypeCategories::Type Category =
		FAssetToolsModule::GetModule().Get().RegisterAdvancedAssetCategory(
			FName(TEXT("Example")), FText::FromString("Example"));

	CacheAction = MakeShared<FCustomNormalDistributionActions>(Category);
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(CacheAction.ToSharedRef());
}

void FCustomDataTypeEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(CacheAction.ToSharedRef());
	}
	IModuleInterface::ShutdownModule();
}