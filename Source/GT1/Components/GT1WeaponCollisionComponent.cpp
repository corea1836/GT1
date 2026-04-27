#include "Components/GT1WeaponCollisionComponent.h"

UGT1WeaponCollisionComponent::UGT1WeaponCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

}

void UGT1WeaponCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGT1WeaponCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsCollisionEnabled)
	{
		CollisionTrace();
	}
}

void UGT1WeaponCollisionComponent::TurnOnCollision()
{
	AlreadyHitActors.Empty();
	bIsCollisionEnabled = true;
}

void UGT1WeaponCollisionComponent::TurnOffCollision()
{
	bIsCollisionEnabled = false;
}

void UGT1WeaponCollisionComponent::SetWeaponMesh(UPrimitiveComponent* MeshComponent)
{
	WeaponMesh = MeshComponent;
}

void UGT1WeaponCollisionComponent::AddIgnoredActor(AActor* Actor)
{
	IgnoredActors.Add(Actor);
}

void UGT1WeaponCollisionComponent::RemoveIgnoredActor(AActor* Actor)
{
	IgnoredActors.Remove(Actor);
}

bool UGT1WeaponCollisionComponent::CanHitActor(AActor* Actor) const
{
	return AlreadyHitActors.Contains(Actor) == false;
}

void UGT1WeaponCollisionComponent::CollisionTrace()
{
	TArray<FHitResult> OutHits;
	
	const FVector Start = WeaponMesh->GetSocketLocation(TraceStartSocketName);
	const FVector End = WeaponMesh->GetSocketLocation(TraceEndSocketName);
	
	bool const bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetOwner(),
		Start,
		End,
		TraceRadius,
		TraceObjectTypes,
		false,
		IgnoredActors,
		DrawDebugType,
		OutHits,
		true);
	
	if (bHit)
	{
		for (const FHitResult& Hit : OutHits) 
		{
			AActor* HitActor = Hit.GetActor();
			
			if (HitActor == nullptr)
			{
				continue;
			}
			
			if (CanHitActor(HitActor))
			{
				AlreadyHitActors.Add(HitActor);
				
				if (OnHitActor.IsBound())
				{
					OnHitActor.Broadcast(Hit);
				}
			}
		}
	}
}

