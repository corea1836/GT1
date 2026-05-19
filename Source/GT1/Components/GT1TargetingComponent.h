#pragma once

#include "CoreMinimal.h"
#include "GT1Define.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GT1TargetingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GT1_API UGT1TargetingComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetRadius = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FaceLockOnRotationSpeed = 20.f;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;
	
protected:
	UPROPERTY()
	TObjectPtr<ACharacter> Character;
	
	UPROPERTY()
	TObjectPtr<class UCameraComponent> Camera;
	
	UPROPERTY()
	TObjectPtr<AActor> LockedTargetActor;
	
	bool bIsLockOn = false;

public:	
	UGT1TargetingComponent();
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void ToggleLockOn();
	
	void SwitchingLockOnActor(ESwitchingDirection InDirection);
	
	FORCEINLINE bool IsLockOn() const { return bIsLockOn; }
	
protected:
	void FindTargets(OUT TArray<AActor*>& OutTargetingActors) const;
	
	AActor* FindClosestTarget(TArray<AActor*>& InTargets, ESwitchingDirection InDirection = ESwitchingDirection::None) const;
	
	void OrientCamera() const;
	
	void OrientMovement() const;
	
	void FaceLockOnActor() const;
	
	void LockOnTarget();
	
	void StopLockOn();
};
