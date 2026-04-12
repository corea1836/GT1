#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "GT1StateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GT1_API UGT1StateComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// 이동키 입력 상태 관리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	bool bMovementInputEnabled = true;
	
	// 캐릭터 현재 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	FGameplayTag CurrentState;
	
public:	
	UGT1StateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	UFUNCTION()
	void MovementInputEnableAction();
	
public:
	// 이동 입력상태 활성화인지?
	FORCEINLINE bool MovementInputEnabled() const { return bMovementInputEnabled; }
	
	void ToggleMovementInput(bool bEnabled, float Duration = 0.f);
	
	FORCEINLINE void SetState(const FGameplayTag NewState) { CurrentState = NewState; }
	FORCEINLINE FGameplayTag GetCurrentState() const { return CurrentState; }
	
	bool IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const;
	
};
