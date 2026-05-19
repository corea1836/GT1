#include "Items/GT1PickupItem.h"

#include "Equipments/GT1Equipment.h"

AGT1PickupItem::AGT1PickupItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupItemMesh"));
	SetRootComponent(Mesh);
}

void AGT1PickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGT1PickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGT1PickupItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (EquipmentClass)
	{
		if (AGT1Equipment* CDO = EquipmentClass->GetDefaultObject<AGT1Equipment>())
		{
			Mesh->SetStaticMesh(CDO->MeshAsset);
			Mesh->SetSimulatePhysics(true);
		}
	}
}

void AGT1PickupItem::Interact(AActor* InteractionActor)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = InteractionActor;
	
	AGT1Equipment* SpawnItem = GetWorld()->SpawnActor<AGT1Equipment>(EquipmentClass, GetActorTransform(), SpawnParams);
	if (SpawnItem)
	{
		SpawnItem->EquipItem();
		Destroy();
	}
}

