#include "Animations/GT1AnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Characters/GT1Character.h"
#include "Components/GT1CombatComponent.h"
#include "Components/GT1StateComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGT1AnimInstance::UGT1AnimInstance()
{
}

void UGT1AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Character = Cast<AGT1Character>(GetOwningActor());
	
	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
		CombatComponent = Character->GetComponentByClass<UGT1CombatComponent>();
	}
}

void UGT1AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (Character == nullptr || MovementComponent == nullptr) return;
	
	Velocity = MovementComponent->Velocity;
	GroundSpeed = Velocity.Size2D();
	
	bIsAccelerating = Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f;
	
	bShouldMove = GroundSpeed > 3.f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector;
	
	bIsFalling = MovementComponent->IsFalling();
	
	if (bIsFalling) LandingSpeed = Velocity.Z;
	
	bCombatEnabled = CombatComponent->IsCombatEnabled();
	bSprinting = Character->IsSprinting();
	
	if (bShouldMove)
		StopEntrySpeed = GroundSpeed;
	
	if (bShouldMove)
	{
		if (bSprinting)
			SprintReleaseTimer = 0.15f;
		else
			SprintReleaseTimer -= DeltaSeconds;

		bWasSprintingWhenMoving = (SprintReleaseTimer > 0.f);
	}
	
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Character->GetActorRotation());
	
	GEngine->AddOnScreenDebugMessage(3, 1.5f, FColor::Red, FString::Printf(TEXT("Ground Speed : %f"), GroundSpeed));
	GEngine->AddOnScreenDebugMessage(3, 1.5f, FColor::Cyan, FString::Printf(TEXT("StopEntrySpeed : %f"), StopEntrySpeed));
	
}

void UGT1AnimInstance::AnimNotify_ResetMovementInput()
{
	if (AGT1Character* LocalCharacter = Cast<AGT1Character>(GetOwningActor()))
	{
		LocalCharacter->GetStateComponent()->ToggleMovementInput(true);
	}
}

void UGT1AnimInstance::AnimNotify_ResetState()
{
	if (AGT1Character* LocalCharacter = Cast<AGT1Character>(GetOwningActor()))
	{
		LocalCharacter->GetStateComponent()->ClearState();
	}
}
