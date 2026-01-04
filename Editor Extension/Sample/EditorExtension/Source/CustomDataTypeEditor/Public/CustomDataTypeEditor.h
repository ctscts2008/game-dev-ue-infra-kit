
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "CustomNormalDistributionActions.h"

class FCustomDataTypeEditorModule : public IModuleInterface
{
public:

virtual void StartupModule() override;
virtual void ShutdownModule() override;

private:
	TSharedPtr<FCustomNormalDistributionActions> CacheAction;
};