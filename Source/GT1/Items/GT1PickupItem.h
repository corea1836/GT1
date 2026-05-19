#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GT1Interact.h"
#include "GT1PickupItem.generated.h"

UCLASS()
class GT1_API AGT1PickupItem : public AActor, public IGT1Interact
{
	GENERATED_BODY()
	
public:	
	AGT1PickupItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void Interact(AActor* InteractionActor) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Item")
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSubclassOf<class AGT1Equipment> EquipmentClass;
};
