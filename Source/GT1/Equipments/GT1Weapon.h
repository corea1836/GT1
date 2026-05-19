#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Equipments/GT1Equipment.h"
#include "GT1Weapon.generated.h"

UCLASS()
class GT1_API AGT1Weapon : public AGT1Equipment
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment | Socket")
	FName EquipSocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment | Socket")
	FName UnequipSocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment | Animation")
	TObjectPtr<class UGT1MontageActionData> MontageActionData;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGT1WeaponCollisionComponent> WeaponCollisionComponent;
	
protected:
	UPROPERTY()
	TObjectPtr<class UGT1CombatComponent> CombatComponent;
	
protected:
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> StaminaCostMap;
	
	UPROPERTY(EditAnywhere)
	float BaseDamage = 15.f;
	
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> DamageMultiplyerMap;

public:
	AGT1Weapon();
	
public:
	virtual void EquipItem() override;
	
	TObjectPtr<UAnimMontage> GetMontageForTag(const struct FGameplayTag& Tag, const int Index = 0) const;
	
	float GetStaminaCost(const FGameplayTag& InTag) const;
	float GetAttackDamage() const;

	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; }
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName; }
	FORCEINLINE UGT1WeaponCollisionComponent* GetCollisionComponent() const { return WeaponCollisionComponent; }
	
public:
	void OnHitActor(const FHitResult& Hit);
};
