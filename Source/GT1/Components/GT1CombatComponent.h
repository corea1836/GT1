#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Equipments/GT1Weapon.h"
#include "GT1CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GT1_API UGT1CombatComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class AGT1Weapon> MainWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AGT1Weapon> DefaultWeaponClass;
	
	UPROPERTY()
	TObjectPtr<AGT1Weapon> DefaultWeapon;
	
	UPROPERTY(EditAnywhere)
	bool bCombatEnabled = false;
	
	UPROPERTY(VisibleAnywhere)
	FGameplayTag LastAttackType;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TObjectPtr<UStaticMeshComponent> ScabbardMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TObjectPtr<UStaticMesh> ScabbardFullMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TObjectPtr<UStaticMesh> ScabbardEmptyMesh;
	
	
	
public:	
	UGT1CombatComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetWeapon(AGT1Weapon* NewWeapon);
	
public:
	FORCEINLINE bool IsCombatEnabled() const { return bCombatEnabled; }
	FORCEINLINE void SetCombatEnabled(const bool bEnabled) { bCombatEnabled = bEnabled; }
	FORCEINLINE AGT1Weapon* GetMainWeapon() const { return MainWeapon; }
	
	FORCEINLINE bool IsDefaultWeapon(const AGT1Weapon* Weapon) const { return Weapon && Weapon->IsA(DefaultWeaponClass); }
	
	FORCEINLINE void SetDefaultWeaponVisual(const bool bDrawn)
	{
		if (ScabbardMesh)
		{
			ScabbardMesh->SetStaticMesh(bDrawn ? ScabbardFullMesh : ScabbardEmptyMesh);
		}
	}
	
	FORCEINLINE FGameplayTag GetLastAttackType() const { return LastAttackType; }
	FORCEINLINE void SetLastAttackType(const FGameplayTag& NewAttackTypeTag) { LastAttackType = NewAttackTypeTag; }
};
