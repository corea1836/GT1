#include "Components/GT1TargetingComponent.h"

#include "GT1Define.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/GT1Targeting.h"
#include "Kismet/KismetMathLibrary.h"


UGT1TargetingComponent::UGT1TargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UGT1TargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		Camera = Character->GetComponentByClass<UCameraComponent>();
	}
}

void UGT1TargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsLockOn == false)
	{
		return;
	}
	
	if (IsValid(Character) == false)
	{
		return;
	}
	
	if (IsValid(LockedTargetActor) == false)
	{
		return;
	}
	
	const float Distance = FVector::Distance(Character->GetActorLocation(), LockedTargetActor->GetActorLocation());
	
	if (IGT1Targeting* Targeting = Cast<IGT1Targeting>(LockedTargetActor))
	{
		if (Targeting->CanBeTargeted() == false || Distance > TargetRadius)
		{
			StopLockOn();
		}
		else
		{
			FaceLockOnActor();
		}
	}
}

void UGT1TargetingComponent::ToggleLockOn()
{
	if (bIsLockOn)
	{
		StopLockOn();
	}
	else
	{
		LockOnTarget();
	}
}

void UGT1TargetingComponent::SwitchingLockOnActor(ESwitchingDirection InDirection)
{
	if (::IsValid(LockedTargetActor))
	{
		if (IGT1Targeting* Targeting = Cast<IGT1Targeting>(LockedTargetActor))
		{
			Targeting->OnTargeted(false);
			
			TArray<AActor*> OutTargets;
			FindTargets(OutTargets);
			AActor* TargetActor = FindClosestTarget(OutTargets, InDirection);
			
			if (::IsValid(TargetActor))
			{
				if (IGT1Targeting* NewTargeting =Cast<IGT1Targeting>(TargetActor))
				{
					LockedTargetActor = TargetActor;
					NewTargeting->OnTargeted(true);
				}
			}
			else
			{
				StopLockOn();
			}
		}
	}
}

void UGT1TargetingComponent::FindTargets(TArray<AActor*>& OutTargetingActors) const
{
	TArray<FHitResult> OutHits;
	const FVector Start = Character->GetActorLocation();
	const FVector End = Start;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_TARGETING));
	
	TArray<AActor*> ActorsToIgnore;
	
	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetOwner(),
		Start,
		End,
		TargetRadius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		DrawDebugType,
		OutHits,
		true);
	
	if (bHit)
	{
		for (const FHitResult& HitResult : OutHits)
		{
			AActor* HitActor = HitResult.GetActor();
			if (IGT1Targeting* Targeting = Cast<IGT1Targeting>(HitActor))
			{
				if (Targeting->CanBeTargeted())
				{
					OutTargetingActors.Add(HitActor);
				}
			}
		}
	}
}

AActor* UGT1TargetingComponent::FindClosestTarget(TArray<AActor*>& InTargets, ESwitchingDirection InDirection) const
{
	float TargetCompareValue = 0.f;
	AActor* ClosestTarget = nullptr;
	
	for (const AActor* TargetActor : InTargets)
	{
		if (InDirection != ESwitchingDirection::None && LockedTargetActor == TargetActor)
		{
			continue;;
		}
		
		FHitResult OutHit;
		const FVector Start = Camera->GetComponentLocation();
		const FVector End = TargetActor->GetActorLocation();
		
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(Character);
		
		const bool bHit = UKismetSystemLibrary::LineTraceSingle(
			GetOwner(),
			Start,
			End,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			ActorsToIgnore,
			DrawDebugType,
			OutHit,
			true);
		
		if (!bHit || OutHit.GetActor() != TargetActor)
		{
			continue;
		}
		
		if (bHit)
		{
			if (InDirection == ESwitchingDirection::Left)
			{
				if (FVector::DotProduct(Camera->GetRightVector(), OutHit.Normal) > 0.f == false)
				{
					continue;
				}
			}
			
			if (InDirection == ESwitchingDirection::Right)
			{
				if (FVector::DotProduct(Camera->GetRightVector(), OutHit.Normal) < 0.f == false)
				{
					continue;
				}
			}
			
			AActor* HitActor = OutHit.GetActor();
			
			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), HitActor->GetActorLocation());
			float CheckValue = FVector::DotProduct(Camera->GetForwardVector(), LookAtRotation.Vector());
			
			if (CheckValue > TargetCompareValue)
			{
				TargetCompareValue = CheckValue;
				ClosestTarget = HitActor;
			}
		}
	}
	
	return ClosestTarget;
}

void UGT1TargetingComponent::OrientCamera() const
{
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void UGT1TargetingComponent::OrientMovement() const
{
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void UGT1TargetingComponent::FaceLockOnActor() const
{
	const FRotator CurrentControlRotation = Character->GetControlRotation();
	
	const FVector TargetLocation = LockedTargetActor->GetActorLocation() - FVector(0.f, 0.f, 150.f);
	const FRotator TargetLookAtRotation = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), TargetLocation);
	
	FRotator InterpRotation = FMath::RInterpTo(CurrentControlRotation, TargetLookAtRotation, GetWorld()->GetDeltaSeconds(), FaceLockOnRotationSpeed);
	
	Character->GetController()->SetControlRotation(FRotator(InterpRotation.Pitch, InterpRotation.Yaw, InterpRotation.Roll));
	
}

void UGT1TargetingComponent::LockOnTarget()
{
	TArray<AActor*> OutTargets;
	FindTargets(OutTargets);
	AActor* TargetActor = FindClosestTarget(OutTargets);
	
	if (::IsValid(TargetActor))
	{
		LockedTargetActor = TargetActor;
		bIsLockOn = true;
		if (IGT1Targeting* Targeting = Cast<IGT1Targeting>(LockedTargetActor))
		{
			Targeting->OnTargeted(bIsLockOn);
		}
		
		OrientCamera();
	}
}

void UGT1TargetingComponent::StopLockOn()
{
	bIsLockOn = false;
	if (IGT1Targeting* Targeting = Cast<IGT1Targeting>(LockedTargetActor))
	{
		Targeting->OnTargeted(bIsLockOn);
	}
	LockedTargetActor = nullptr;
	OrientMovement();
}

