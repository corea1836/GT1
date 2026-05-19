#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GT1WeaponCollisionComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitActor, const FHitResult&)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), meta=(Displayname="Weapon Collision"))
class GT1_API UGT1WeaponCollisionComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	FOnHitActor OnHitActor;
	
protected:
	UPROPERTY(EditAnywhere)
	FName TraceStartSocketName;
	
	UPROPERTY(EditAnywhere)
	FName TraceEndSocketName;
	
protected:
	UPROPERTY(EditAnywhere)
	float TraceRadius = 20.f;
	
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	
	UPROPERTY(EditAnywhere)
	TArray<AActor*> IgnoredActors;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;
	
protected:
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> WeaponMesh;
	
	UPROPERTY()
	TArray<AActor*> AlreadyHitActors;
	
	bool bIsCollisionEnabled = false;
	
public:	
	UGT1WeaponCollisionComponent();

protected:
	virtual void BeginPlay() override;

public:	
 	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void TurnOnCollision();
	
	void TurnOffCollision();
	
	void SetWeaponMesh(UPrimitiveComponent* MeshComponent);
	
	void AddIgnoredActor(AActor* Actor);
	
	void RemoveIgnoredActor(AActor* Actor);
	
protected:
	bool CanHitActor(AActor* Actor) const;
	
	void CollisionTrace();
		
};
