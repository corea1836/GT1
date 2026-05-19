#include "Components/GT1CombatComponent.h"

#include "Characters/GT1Character.h"
#include "Equipments/GT1Weapon.h"

UGT1CombatComponent::UGT1CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGT1CombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (ScabbardFullMesh)
	{
		ScabbardMesh = NewObject<UStaticMeshComponent>(GetOwner());
		ScabbardMesh->SetStaticMesh(ScabbardFullMesh);
		ScabbardMesh->RegisterComponent();
		ScabbardMesh->AttachToComponent(
			Cast<ACharacter>(GetOwner())->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			TEXT("blade_unequip_socket"));
	}
	
	if (DefaultWeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		
		DefaultWeapon = GetWorld()->SpawnActor<AGT1Weapon>(DefaultWeaponClass, GetOwner()->GetActorTransform(), SpawnParams);
		
		if (DefaultWeapon)
		{
			DefaultWeapon->SetActorHiddenInGame(true);
			DefaultWeapon->SetActorEnableCollision(false);
		}
		
		MainWeapon = DefaultWeapon;
		AGT1Character* Character = Cast<AGT1Character>(GetOwner());
		MainWeapon->EquipItem();
	}
}


void UGT1CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGT1CombatComponent::SetWeapon(AGT1Weapon* NewWeapon)
{
	MainWeapon = NewWeapon;
}

