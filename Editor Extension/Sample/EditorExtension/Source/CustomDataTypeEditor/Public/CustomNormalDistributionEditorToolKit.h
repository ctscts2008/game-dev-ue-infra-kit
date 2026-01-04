#pragma once

#include "CoreMinimal.h"
#include "CustomNormalDistribution.h"
#include "CustomNormalDistributionActions.h"
#include "Toolkits/AssetEditorToolkit.h"

class FCustomNormalDistributionEditorToolKit : public FAssetEditorToolkit
{
public:

	void InitEditor(const TArray<UObject*>& InObjects) ;

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	virtual FName GetToolkitFName() const override { return "CustomNormalDistributionEditor"; }
	virtual FText GetBaseToolkitName() const override{ return INVTEXT("Custom Normal Distribution Editor"); }
	virtual FString GetWorldCentricTabPrefix() const override { return "Custom Normal Distribution"; }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return {}; }

	float GetMean() const;
	float GetStandardDeviation() const;
	void SetMean(float Mean);
	void SetStandardDeviation(float StandardDeviation);

private:

	UCustomNormalDistribution* NormalDistribution = nullptr;
};