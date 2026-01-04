// ExtendMenuBase.h

#pragma once

#include "Modules/ModuleInterface.h"

/*
 *一拓展编辑器的测试用例
 */
class FExtendMenuBase : public IModuleInterface , public TSharedFromThis<FExtendMenuBase>
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual ~FExtendMenuBase() {}
private:
	//FExtender for Main menu
	void CreateMenuBar();
	TSharedRef<class FExtender>  ExtendAssetContextMenu(const TArray<FString>& SelectedPaths);
	// FExtender for content browsers
	void ExcuteContentBrowserExtend();
    void NewMenu2ButtonAction();
	//tool menu extend
	void ExtendByToolMenus();

	void OnLevelEditorCreatedEvent(TSharedPtr<class ILevelEditor> Editor);

	/* Style 实例入口 */
	void StyleSample();

	void StyleButtonAction();

	/* TCommand 示例 */
	void CommandSample();

private:
	static int32 ClickCount;
	class UMessageClass* MessageDelegate = nullptr;
};