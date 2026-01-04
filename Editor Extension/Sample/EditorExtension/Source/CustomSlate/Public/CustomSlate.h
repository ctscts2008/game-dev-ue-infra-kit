#pragma once

#include "Modules/ModuleManager.h"


class FCustomSlateModule : public IModuleInterface
{

public:
virtual void StartupModule() override;
virtual void ShutdownModule() override;
virtual ~FCustomSlateModule() {}


private:
	void CreateButtonByStyle();
	void ButtonAction();

	void SpawnWindow();

	void SpawnTab();

	void SpawnLayoutTab();

	TSharedPtr<FTabManager> TabManager;
	TSharedPtr<FTabManager::FLayout> TabMananagerLayOut;
	TSharedRef<SDockTab> OnSpawnMainTab(const FSpawnTabArgs& SpawnArgs);
	TSharedRef<SDockTab> OnSpawnSubTab(const FSpawnTabArgs& SpawnArgs);

	FText SubTabName;
};