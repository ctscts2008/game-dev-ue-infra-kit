// ExtendEditorAction.h

#pragma once

#include "Modules/ModuleInterface.h"

class CUSTOMMESSAGE_API FCustomMessageModule  : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual ~FCustomMessageModule() {}

};


