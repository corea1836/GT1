#pragma once

#include "CoreMinimal.h"
#include "GT1Define.h"
#include "Components/ActorComponent.h"
#include "GT1AttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegateOnAttributeChanged, EGT1AttributeType, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GT1_API UGT1AttributeComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	// 스텍 변경 Delegate
	FDelegateOnAttributeChanged OnAttributeChanged;
	
protected:
	UPROPERTY(EditAnywhere, Category="Stamina")
	float BaseStamina = 100.f;
	
	UPROPERTY(EditAnywhere, Category="Stamina")
	float MaxStamina = 100.f;
	
	UPROPERTY(EditAnywhere, Category="Stamina")
	float StaminaRegenRate = 0.5f;
	
	// 스테미너 재충전 타이머 핸들러
	FTimerHandle StaminaRegenTimerHandle;

public:	
	UGT1AttributeComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE float GetBaseStamina() const { return BaseStamina; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }
	
	FORCEINLINE float GetStaminaRatio() const { return BaseStamina / MaxStamina; }
	
	// 스테미너가 충분한지 체크
	bool CheckHasEnoughStamina(float StaminaCost) const;
	
	// 스테미너 차감
	void DecreaseStamina(float StaminaCost);
	
	// 스테미너 재충전/중지 토글
	void ToggleStaminaRegeneration(bool bEnabled, float StartDelay = 2.f);
	
	void BroadcastAttributeOnChanged(EGT1AttributeType AttributeType) const;
	
private:
	void RegenerateStaminaHandler();
		
};
