#include "Components/GT1SoftTargetingComponent.h"

#include "GT1AttributeComponent.h"
#include "GT1Define.h"
#include "Characters/GT1Enemy.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/GT1Targeting.h"
#include "Kismet/KismetMathLibrary.h"

UGT1SoftTargetingComponent::UGT1SoftTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGT1SoftTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Character = Cast<ACharacter>(GetOwner());
}

void UGT1SoftTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!IsValid(Character)) return;
	if (!IsValid(FocusTargetActor)) return;
	
	FaceLockOnActor();
}

void UGT1SoftTargetingComponent::SetTarget(AActor* NewTargetActor)
{
	if (FocusTargetActor == NewTargetActor) return;
	
	AActor* OldTargetActor = FocusTargetActor;
	
	if (IsValid(OldTargetActor))
	{
		if (IGT1Targeting* Targeting = Cast<IGT1Targeting>(OldTargetActor))
		{
			Targeting->OnTargeted(false);
		}
		
		if (UGT1AttributeComponent* AttributeComponent = FocusTargetActor->FindComponentByClass<UGT1AttributeComponent>())
		{
			AttributeComponent->OnDeath.RemoveAll(this);
		}
	}
	
	FocusTargetActor = NewTargetActor;
	
	if (IsValid(FocusTargetActor))
	{
		if (IGT1Targeting* Targeting = Cast<IGT1Targeting>(FocusTargetActor))
		{
			Targeting->OnTargeted(true);
		}
		
		if (UGT1AttributeComponent* AttributeComponent = FocusTargetActor->FindComponentByClass<UGT1AttributeComponent>())
		{
			AttributeComponent->OnDeath.AddUObject(this, &ThisClass::TargetDeath);
		}
	}
	
	Character->GetCharacterMovement()->bOrientRotationToMovement = !IsValid(FocusTargetActor);
}

void UGT1SoftTargetingComponent::ToggleTargeting(bool bEnabled)
{
	if (bEnabled)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(TargetingUpdateTimerHandle) == false)
		{
			GetWorld()->GetTimerManager().SetTimer(TargetingUpdateTimerHandle, this, &ThisClass::UpdateTarget, 1.5f, true);
		}
	} 
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TargetingUpdateTimerHandle);
	}
}

void UGT1SoftTargetingComponent::ActiveTargeting()
{
	ToggleTargeting(true);
	
	UpdateTarget();
}

void UGT1SoftTargetingComponent::DeactiveTargeting()
{
	ToggleTargeting(false);
	
	SetTarget(nullptr);
}

void UGT1SoftTargetingComponent::UpdateTargetByDirection(FVector InDirection)
{
	if (!IsValid(FocusTargetActor))
	{
		SetTarget(nullptr);
	}
	
	TArray<AActor*> OutTargets;
	FindTargets(OutTargets);
	
	if (OutTargets.Num() == 0)
	{
		SetTarget(nullptr);
		return;
	}
	
	AActor* NewTarget = FindClosestTarget(OutTargets, InDirection);
	
	SetTarget(NewTarget);
}

void UGT1SoftTargetingComponent::UpdateTarget()
{
	if (IsValid(FocusTargetActor))
	{
		CheckTargetDistance();
		return;
	}
	
	TArray<AActor*> OutTargets;
	FindTargets(OutTargets);
	
	if (OutTargets.Num() == 0)
	{
		SetTarget(nullptr);
		return;
	}
	
	FVector CharacterForward = Character->GetActorForwardVector();
	AActor* NewTarget = FindClosestTarget(OutTargets, CharacterForward);
	
	SetTarget(NewTarget);
}

void UGT1SoftTargetingComponent::FaceLockOnActor()
{
	const FRotator CurrentCharacterRotation = Character->GetActorRotation();
	FRotator TargetLookAtRotation = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), FocusTargetActor->GetActorLocation());
	TargetLookAtRotation.Pitch = 0.f;
	TargetLookAtRotation.Roll = 0.f;
	
	FRotator InterpRotation = FMath::RInterpTo(CurrentCharacterRotation, TargetLookAtRotation, GetWorld()->GetDeltaSeconds(), FaceLockOnRotationSpeed);
	
	Character->SetActorRotation(InterpRotation);
}

void UGT1SoftTargetingComponent::FindTargets(TArray<AActor*>& OutTargetingActors) const
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
		SearchRadius,
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

AActor* UGT1SoftTargetingComponent::FindClosestTarget(TArray<AActor*>& InTargets, FVector& InDirection) const
{
	float BestScore = -FLT_MAX;
	AActor* BestTarget = nullptr;
	
	const FVector CharacterLocation = Character->GetActorLocation();
	
	for (const AActor* TargetActor : InTargets)
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CharacterLocation, TargetActor->GetActorLocation());
		LookAtRotation.Pitch = 0.0f;
		LookAtRotation.Roll = 0.0f;
		
		float DirectionDot = FVector::DotProduct(InDirection, LookAtRotation.Vector());
		if (DirectionDot < MinDirectionDot) continue;
		
		FHitResult OutHit;
		FVector Start = CharacterLocation;
		FVector End = TargetActor->GetActorLocation();
		
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
		
		AActor* HitActor = OutHit.GetActor();
		
		float Distance = FVector::Dist2D(CharacterLocation, HitActor->GetActorLocation());
		
		float DistanceScore = 1.f - FMath::Clamp(Distance / SearchRadius, 0.f, 1.f);
		float TotalScore = DirectionDot * DirectionWeight + DistanceScore * DistanceWeight;
		
		if (TotalScore > BestScore)
		{
			BestScore = TotalScore;
			BestTarget = HitActor;
		}
	}
	
	return BestTarget;
}

void UGT1SoftTargetingComponent::CheckTargetDistance()
{
	float Distance = FVector::Dist2D(
		Character->GetActorLocation(),
		FocusTargetActor->GetActorLocation());
	
	if (Distance > TargetingBreakDistance)
	{
		SetTarget(nullptr);
	}
}

void UGT1SoftTargetingComponent::TargetDeath()
{
	SetTarget(nullptr);
}

