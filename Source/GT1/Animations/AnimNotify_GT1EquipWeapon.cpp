#include "Animations/AnimNotify_GT1EquipWeapon.h"

#include "GT1GameplayTags.h"
#include "Components/GT1CombatComponent.h"
#include "Equipments/GT1Weapon.h"

UAnimNotify_GT1EquipWeapon::UAnimNotify_GT1EquipWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UAnimNotify_GT1EquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (const AActor* Owner = MeshComp->GetOwner())
	{
		if (UGT1CombatComponent* CombatComponent = Owner->GetComponentByClass<UGT1CombatComponent>())
		{
			if (AGT1Weapon* MainWeapon = CombatComponent->GetMainWeapon())
			{
				bool bCombatEnabled = CombatComponent->IsCombatEnabled();
				FName WeaponSocketName;
				
				if (MontageActionTag == GT1GameplayTags::Character_Action_Equip)
				{
					bCombatEnabled = true;
					WeaponSocketName = MainWeapon->GetEquipSocketName();
				}
				else if (MontageActionTag == GT1GameplayTags::Character_Action_Unequip)
				{
					bCombatEnabled = false;
					WeaponSocketName = MainWeapon->GetUnequipSocketName();
				}
				
				CombatComponent->SetCombatEnabled(bCombatEnabled);
				MainWeapon->AttachToOwner(WeaponSocketName);
				
				if (CombatComponent->IsDefaultWeapon(MainWeapon))
				{
					CombatComponent->SetDefaultWeaponVisual(bCombatEnabled ? false : true);
					MainWeapon->SetActorHiddenInGame(bCombatEnabled ? false : true);
					MainWeapon->SetActorEnableCollision(bCombatEnabled ? true : false);
				}
			}
		}
		
	}
}
