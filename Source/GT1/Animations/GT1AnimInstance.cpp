#include "Animations/GT1AnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGT1AnimInstance::UGT1AnimInstance()
{
}

void UGT1AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Character = Cast<ACharacter>(GetOwningActor());
	
	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
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
	
	GEngine->AddOnScreenDebugMessage(3, 1.5f, FColor::Red, FString::Printf(TEXT("Landing Speed : %f"), LandingSpeed));
	
}
