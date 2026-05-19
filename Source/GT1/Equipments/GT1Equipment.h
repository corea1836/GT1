#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GT1Equipment.generated.h"

UCLASS()
class GT1_API AGT1Equipment : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="Equipment | Mesh")
	TObjectPtr<UStaticMesh> MeshAsset;
	
	UPROPERTY(EditAnywhere, Category="Equipment | Mesh")
	TObjectPtr<UStaticMeshComponent> Mesh;
	
public:	
	AGT1Equipment();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:
	virtual void EquipItem();
	
	virtual void UnequipItem();
	
	virtual void AttachToOwner(FName SocketName);
};