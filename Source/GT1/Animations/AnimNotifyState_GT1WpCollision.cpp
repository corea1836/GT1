#include "Animations/AnimNotifyState_GT1WpCollision.h"

#include "Components/GT1CombatComponent.h"
#include "Components/GT1WeaponCollisionComponent.h"

UAnimNotifyState_GT1WpCollision::UAnimNotifyState_GT1WpCollision(const FObjectInitializer& ObjectInitializer)
{
}

void UAnimNotifyState_GT1WpCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (const UGT1CombatComponent* CombatComponent = OwnerActor->GetComponentByClass<UGT1CombatComponent>())
		{
			const AGT1Weapon* Weapon = CombatComponent->GetMainWeapon();
			
			if (::IsValid(Weapon))
			{
				Weapon->GetCollisionComponent()->TurnOnCollision();
			}
		}
	}
}

void UAnimNotifyState_GT1WpCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (const UGT1CombatComponent* CombatComponent = OwnerActor->GetComponentByClass<UGT1CombatComponent>())
		{
			const AGT1Weapon* Weapon = CombatComponent->GetMainWeapon();
			
			if (::IsValid(Weapon))
			{
				Weapon->GetCollisionComponent()->TurnOffCollision();
			}
		}
	}
}
