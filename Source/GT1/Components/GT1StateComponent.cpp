#include "Components/GT1StateComponent.h"

#include "Kismet/KismetSystemLibrary.h"

UGT1StateComponent::UGT1StateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGT1StateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGT1StateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGT1StateComponent::MovementInputEnableAction()
{
	bMovementInputEnabled = true;
	ClearState();
}

void UGT1StateComponent::ToggleMovementInput(bool bEnabled, float Duration)
{
	if (bEnabled)
	{
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		LatentAction.ExecutionFunction = "MovementInputEnableAction";
		LatentAction.Linkage = 0;
		LatentAction.UUID = 0;
		
		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), Duration, LatentAction);
	}
	else
	{
		bMovementInputEnabled = false;
	}
}

void UGT1StateComponent::ClearState()
{
	CurrentState = FGameplayTag::EmptyTag;
}

bool UGT1StateComponent::IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const
{
	return TagsToCheck.HasTagExact(CurrentState);
}

