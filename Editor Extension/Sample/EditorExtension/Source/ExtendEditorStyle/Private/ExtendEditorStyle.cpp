// ExtendMenuBase.cpp

#pragma once

#include "ExtendEditorStyle.h"

#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

IMPLEMENT_MODULE(FExtendEditorStyle, ExtendEditorStyle)

void FExtendEditorStyle::StartupModule()
{
	IModuleInterface::StartupModule();

	Initialize();
}

void FExtendEditorStyle::ShutdownModule()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);
	StyleSet.Reset();

	IModuleInterface::ShutdownModule();
}

FName FExtendEditorStyle::GetStyleSetName()
{
	return StyleSetName;
}

void FExtendEditorStyle::Initialize()
{
	if(!StyleSet.IsValid())
	{
		StyleSet = CreateSlateStyleSet();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
	}
}

TSharedRef<FSlateStyleSet> FExtendEditorStyle::CreateSlateStyleSet()
{
	TSharedRef<FSlateStyleSet> SlateStyleSet = MakeShareable(new FSlateStyleSet(StyleSetName));

	const FString RootPath = FPaths::ProjectDir() + TEXT("/Resource/");
	SlateStyleSet->SetContentRoot(RootPath);
	
	// 添加Style
	{
		const FVector2D IconeSize(16.f, 16.f);
		FSlateImageBrush* SlateImageBrush = 
		new FSlateImageBrush(RootPath + TEXT("MyIcon.jpg"), IconeSize);

		SlateStyleSet->Set("AliceTool", SlateImageBrush);
	}

	{
		const FVector2D IconeSize(16.f, 16.f);
		FSlateImageBrush* SlateImageBrush = 
		new FSlateImageBrush(RootPath + TEXT("MyIcon.jpg"), IconeSize);

		SlateStyleSet->Set("ExtendEditorCommands.PrintLog", SlateImageBrush);
	}
	
	return SlateStyleSet;
}




