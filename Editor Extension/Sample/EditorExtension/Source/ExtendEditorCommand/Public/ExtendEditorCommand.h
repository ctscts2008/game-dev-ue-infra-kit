// ExtendEditorCommand.h

#pragma once

#include "Modules/ModuleInterface.h"

class EXTENDEDITORCOMMAND_API  FExtendEditorCommandModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual ~FExtendEditorCommandModule() override {}
};