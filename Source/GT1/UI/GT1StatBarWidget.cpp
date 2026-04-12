#include "UI/GT1StatBarWidget.h"
#include "Components/ProgressBar.h"

UGT1StatBarWidget::UGT1StatBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGT1StatBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if (StatBar)
	{
		StatBar->SetFillColorAndOpacity(FillColorAndOpacity);
	}
}

void UGT1StatBarWidget::SetRatio(float Ratio) const
{
	if (StatBar)
	{
		StatBar->SetPercent(Ratio);
	}
}
