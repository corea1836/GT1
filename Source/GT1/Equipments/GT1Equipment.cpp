#include "Equipments/GT1Equipment.h"

AGT1Equipment::AGT1Equipment()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipmentMesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void AGT1Equipment::BeginPlay()
{
	Super::BeginPlay();
}

void AGT1Equipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGT1Equipment::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (MeshAsset)
	{
		Mesh->SetStaticMesh(MeshAsset);
	}
}

void AGT1Equipment::EquipItem()
{
}

void AGT1Equipment::UnequipItem()
{
}

void AGT1Equipment::AttachToOwner(FName SocketName)
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh())
		{
			AttachToComponent(CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), SocketName);
		}
	}
}

