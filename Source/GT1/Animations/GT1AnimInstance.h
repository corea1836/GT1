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
	TObjectPtr<ACharacter> Character;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reference")
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;
	
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
	
public:
	UGT1AnimInstance();
	
	// 초기화 시 호출
	virtual void NativeInitializeAnimation() override;
	// 매 틱마다 호출
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
