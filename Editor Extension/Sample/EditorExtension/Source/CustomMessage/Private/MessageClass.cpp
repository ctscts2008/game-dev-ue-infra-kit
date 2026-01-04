// ExtendEditorCommand.cpp 

#pragma once

#include "MessageClass.h"

#include "LevelEditorViewport.h"
#include "Engine/ViewportStatsSubsystem.h"

bool UMessageClass::MESSAAGECallBack(FText& OutText, FLinearColor& OutColor)
{
	OutText = Text;
	OutColor = Color;
	return bShowDisplay;
}
void UMessageClass::SetMessage(FText InText, FLinearColor InColor)
{
	Text = InText;
	Color = InColor;
}

void UMessageClass::ToggleDisplay()
{
	bShowDisplay = !bShowDisplay;
}

bool UMessageClass::IsBind()
{
	return bIsBind;
}

void UMessageClass::BindToVpStatsSubsystem()
{
	if(UViewportStatsSubsystem* ViewportSubsystem = 
		GEditor->GetLevelViewportClients()[0]->GetWorld()->GetSubsystem<UViewportStatsSubsystem>())
	{
		FViewportDisplayCallback Callback;
		Callback.BindDynamic(this, &UMessageClass::MESSAAGECallBack);
		ViewportSubsystem->AddDisplayDelegate(Callback);
	}

	bIsBind = true;
}