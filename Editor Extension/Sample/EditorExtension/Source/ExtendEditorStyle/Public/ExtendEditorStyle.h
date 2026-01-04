// ExtendMenuBase.h

#pragma once

#include "Modules/ModuleInterface.h"

class FSlateStyleSet;
/*
 *一拓展编辑器的测试用例
 */
class  FExtendEditorStyle : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual ~FExtendEditorStyle() {}


public:
	static EXTENDEDITORSTYLE_API TSharedPtr<FSlateStyleSet> GetStyleSet(){return  StyleSet;}
private:
	inline static TSharedPtr<FSlateStyleSet> StyleSet = nullptr;
	inline static const FName StyleSetName = FName("ExtendEditorStyle");

public:

	static EXTENDEDITORSTYLE_API FName GetStyleSetName();

private:
	
	void Initialize();
	TSharedRef<FSlateStyleSet> CreateSlateStyleSet();
};