#pragma once


#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class FCustomNormalDistributionActions : public FAssetTypeActions_Base
{
	public:
	FCustomNormalDistributionActions(EAssetTypeCategories::Type InAssetCategory);

	virtual UClass* GetSupportedClass() const override;
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;

	virtual void GetActions(const TArray<UObject*>& InObjects, class FMenuBuilder& MenuBuilder) override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;


private:
	EAssetTypeCategories::Type AssetCategory;	
};