#include "Components/GT1AttributeComponent.h"

UGT1AttributeComponent::UGT1AttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGT1AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGT1AttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UGT1AttributeComponent::CheckHasEnoughStamina(float StaminaCost) const
{
	return BaseStamina >= StaminaCost;
}

void UGT1AttributeComponent::DecreaseStamina(float StaminaCost)
{
	BaseStamina = FMath::Clamp(BaseStamina - StaminaCost, 0.f, MaxStamina);
	
	BroadcastAttributeOnChanged(EGT1AttributeType::Stamina);
}

void UGT1AttributeComponent::ToggleStaminaRegeneration(bool bEnabled, float StartDelay)
{
	if (bEnabled)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRegenTimerHandle) == false)
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &ThisClass::RegenerateStaminaHandler, 0.1f, true, StartDelay);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	}
}

void UGT1AttributeComponent::BroadcastAttributeOnChanged(EGT1AttributeType InAttributeType) const
{
	if (OnAttributeChanged.IsBound())
	{
		float Ratio = 0.f;
		switch (InAttributeType)
		{
		case EGT1AttributeType::Stamina:
			Ratio = GetStaminaRatio();
			break;
		case EGT1AttributeType::Health:
			break;
		}
		
		OnAttributeChanged.Broadcast(InAttributeType, Ratio);
	}
}

void UGT1AttributeComponent::RegenerateStaminaHandler()
{
	BaseStamina = FMath::Clamp(BaseStamina + StaminaRegenRate, 0.f, MaxStamina);
	
	BroadcastAttributeOnChanged(EGT1AttributeType::Stamina);
	
	if (BaseStamina >= MaxStamina)
	{
		ToggleStaminaRegeneration(false);
	}
}

