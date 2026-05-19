#include "Equipments/GT1Weapon.h"

#include "GT1GameplayTags.h"
#include "Components/GT1CombatComponent.h"
#include "Components/GT1WeaponCollisionComponent.h"
#include "Data/GT1MontageActionData.h"
#include "Kismet/GameplayStatics.h"

AGT1Weapon::AGT1Weapon()
{
	WeaponCollisionComponent = CreateDefaultSubobject<UGT1WeaponCollisionComponent>(TEXT("WeaponCollisionComponent"));
	WeaponCollisionComponent->OnHitActor.AddUObject(this, &ThisClass::OnHitActor);
	
	StaminaCostMap.Add(GT1GameplayTags::Character_Attack_Light, 7.f);	
	StaminaCostMap.Add(GT1GameplayTags::Character_Attack_Heavy, 9.f);	

	DamageMultiplyerMap.Add(GT1GameplayTags::Character_Attack_Heavy, 1.8f);
}

void AGT1Weapon::EquipItem()
{
	Super::EquipItem();
	
	CombatComponent = GetOwner()->GetComponentByClass<UGT1CombatComponent>();
	
	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);
		
		const FName AttachSocket = CombatComponent->IsCombatEnabled() ? EquipSocketName : UnequipSocketName;
		AttachToOwner(AttachSocket);
		
		WeaponCollisionComponent->SetWeaponMesh(Mesh);
		WeaponCollisionComponent->AddIgnoredActor(GetOwner());
	}
}

TObjectPtr<UAnimMontage> AGT1Weapon::GetMontageForTag(const struct FGameplayTag& Tag, const int Index) const
{
	return MontageActionData->GetMontageForTag(Tag, Index);
}

float AGT1Weapon::GetStaminaCost(const FGameplayTag& InTag) const
{
	if (StaminaCostMap.Contains(InTag))
	{
		return StaminaCostMap[InTag];
	}
	
	return 0.f;
}

float AGT1Weapon::GetAttackDamage() const
{
	if (const AActor* OwnerActor = GetOwner())
	{
		const FGameplayTag LastAttackType = CombatComponent->GetLastAttackType();
		
		if (DamageMultiplyerMap.Contains(LastAttackType))
		{
			const float Multiplier = DamageMultiplyerMap[LastAttackType];
			return BaseDamage * Multiplier;
		}
	}
	
	return BaseDamage;
}

void AGT1Weapon::OnHitActor(const FHitResult& Hit)
{
	AActor* TargetActor = Hit.GetActor();
	
	FVector DamageDirection = GetOwner()->GetActorForwardVector();
	
	float AttackDamage = GetAttackDamage();
	
	UGameplayStatics::ApplyPointDamage(
		TargetActor,
		AttackDamage,
		DamageDirection,
		Hit,
		GetOwner()->GetInstigatorController(),
		this,
		nullptr);
	
}

