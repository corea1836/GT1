#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GT1AnimInstance.generated.h"

UCLASS()
class GT1_API UGT1AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
// 캐싱할 컴포넌트
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reference")
	TObjectPtr<class AGT1Character> Character;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reference")
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reference")
	TObjectPtr<class UGT1CombatComponent> CombatComponent;
	
// 매 틱마다 업데이트할 상태	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	FVector Velocity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	float GroundSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	bool bIsAccelerating;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	bool bShouldMove;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	bool bIsFalling;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	float LandingSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	float Direction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat Data")
	bool bCombatEnabled;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat Data")
	bool bSprinting;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat Data")
	float StopEntrySpeed;
	
	UPROPERTY()
	float SprintReleaseTimer = 0.f;

	UPROPERTY(BlueprintReadOnly)
	bool bWasSprintingWhenMoving = false;
	
public:
	UGT1AnimInstance();
	
	// 초기화 시 호출
	virtual void NativeInitializeAnimation() override;
	// 매 틱마다 호출
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	// Anim Notify 처리
	UFUNCTION()
	void AnimNotify_ResetMovementInput();
	
	UFUNCTION()
	void AnimNotify_ResetState();
};
