#include "UI/GT1PlayerHUDWidget.h"
#include "GT1StatBarWidget.h"
#include "Components/GT1AttributeComponent.h"

UGT1PlayerHUDWidget::UGT1PlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGT1PlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		if (UGT1AttributeComponent* AttributeComponent = OwningPawn->GetComponentByClass<UGT1AttributeComponent>())
		{
			AttributeComponent->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChanged);
			AttributeComponent->BroadcastAttributeOnChanged(EGT1AttributeType::Stamina);
		}
	}
}

void UGT1PlayerHUDWidget::OnAttributeChanged(EGT1AttributeType AttributeType, float InValue)
{
	switch (AttributeType)
	{
	case EGT1AttributeType::Stamina:
		StaminaBarWidget->SetRatio(InValue);
		break;
	case EGT1AttributeType::Health:
		break;
	}
}
