#pragma once
#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"   

DECLARE_DELEGATE_OneParam(FOnMeanChanged, float)
DECLARE_DELEGATE_OneParam(FOnStandardDeviationChanged, float)

class SCustomNormalDistributionWidget : public SLeafWidget
{
	public:

	SLATE_BEGIN_ARGS(SCustomNormalDistributionWidget)
		:_Mean(0.5f)
		,_StandardDeviation(0.2f)
		{}

		SLATE_ATTRIBUTE(float,  Mean)
		SLATE_ATTRIBUTE(float,  StandardDeviation)
		SLATE_EVENT(FOnMeanChanged, OnMeanChanged)
		SLATE_EVENT(FOnStandardDeviationChanged, OnStandardDeviationChanged)
	SLATE_END_ARGS()

public:

	void Construct(const FArguments& args);

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;

	virtual FReply  OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
private:

	TAttribute<float> Mean;
	TAttribute<float> StandardDeviation;

	FOnMeanChanged OnMeanChanged;
	FOnStandardDeviationChanged OnStandardDeviationChanged;

	FTransform2D GetPointsTransform(const FGeometry& AllocatedGeometry) const;
};