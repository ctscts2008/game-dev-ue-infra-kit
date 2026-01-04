// MessageClass.h

#pragma once

#include "CoreMinimal.h"
#include "MessageClass.generated.h"

UCLASS()
class CUSTOMMESSAGE_API UMessageClass : public UObject
{
	GENERATED_BODY()

	public:
	UFUNCTION()
	bool MESSAAGECallBack(FText& OutText, FLinearColor& OutColor);

	void SetMessage( FText InText,  FLinearColor InColor);

	void ToggleDisplay();

	void BindToVpStatsSubsystem();

	bool IsBind();

private:

	FText Text = FText();
	FLinearColor Color = FLinearColor::White;
	bool bShowDisplay = false;
	
	bool bIsBind = false;
	
};