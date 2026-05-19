#pragma once

#include "CoreMinimal.h"
#include "GT1Define.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GT1SoftTargetingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GT1_API UGT1SoftTargetingComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SearchRadius = 1500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetingBreakDistance = 2000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float UpdateInterval = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinDirectionDot = 0.3f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DirectionWeight = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DistanceWeight = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InputDeadZone = 0.2f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FaceLockOnRotationSpeed = 20.f;
	
	FTimerHandle TargetingUpdateTimerHandle;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;
	
protected:
	UPROPERTY()
	TObjectPtr<ACharacter> Character;

	UPROPERTY()
	TObjectPtr<AActor> FocusTargetActor;

public:	
	UGT1SoftTargetingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetTarget(AActor* NewTargetActor);
	
	void ToggleTargeting(bool bEnabled);
	
	void ActiveTargeting();
	
	void DeactiveTargeting();
	
	void UpdateTargetByDirection(FVector InDirection);
	
protected:
	void UpdateTarget();
	
	void FaceLockOnActor();
	
	void FindTargets(TArray<AActor*>& OutTargetingActors) const;
	
	AActor* FindClosestTarget(TArray<AActor*>& InTargets, FVector& InDirection) const;
	
	void CheckTargetDistance();
	
	void TargetDeath();
};
