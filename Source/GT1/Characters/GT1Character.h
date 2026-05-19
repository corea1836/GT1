#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "GT1Character.generated.h"

UCLASS()
class GT1_API AGT1Character : public ACharacter
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess=true))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UCameraComponent> FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> SprintAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> RollingAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> ToggleCombatAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> AttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> HeavyAttackAction;
	
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	// TObjectPtr<UInputAction> LockOnTargetAction;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	// TObjectPtr<UInputAction> LeftTargetAction;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	// TObjectPtr<UInputAction> RightTargetAction;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	TObjectPtr<class UGT1AttributeComponent> AttributeComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	TObjectPtr<class UGT1StateComponent> StateComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	TObjectPtr<class UGT1CombatComponent> CombatComponent;
	
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	// TObjectPtr<class UGT1TargetingComponent> TargetingComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	TObjectPtr<class UGT1SoftTargetingComponent> TargetingComponent;
	
protected:
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
	
	UPROPERTY()
	TObjectPtr<class UGT1PlayerHUDWidget> PlayerHUDWidget;
	
protected:
	// 질주 속도
	UPROPERTY(EditAnywhere, Category="Sprinting")
	float SprintingSpeed = 750.f;
	
	// 일반 속도
	UPROPERTY(EditAnywhere, Category="Sprinting")
	float NormalSpeed = 200.f;
	
	UPROPERTY(EditAnywhere, Category="Sprinting")
	bool bSprinting = false;
	
protected:
	bool bComboSequenceRunning = false;

	bool bCanComboInput = false;

	int32 ComboCounter = 0;
	
	bool bSavedComboInput = false;
	
	FTimerHandle ComboResetTimerHandle;
	
	
protected:
	UPROPERTY(EditAnywhere, Category="Montage")
	TObjectPtr<UAnimMontage> RollingMontage;
	
public:
	AGT1Character();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	FORCEINLINE UGT1StateComponent* GetStateComponent() const { return StateComponent; }
	FORCEINLINE bool IsSprinting() const { return bSprinting; }
	
protected:
	bool IsMoving() const;
	bool CanToggleCombat() const;
	
	virtual void Jump() override;
	virtual void Landed(const FHitResult&  Hit) override;
	
	void Move(const FInputActionValue& Values);
	void Look(const FInputActionValue& Values);
	
	void Sprinting();
	
	void StopSprinting();
	
	bool CanRolling() const;
	void Rolling();
	
	void Interact();
	
	void ToggleCombat();
	void AutoToggleCombat();
	
	void Attack();
	void HeavyAttack();
	
	// void LockOnTarget();
	// void LeftTarget();
	// void RightTarget();
	//
protected:
	FGameplayTag GetAttackPerform() const;
	
	bool CanPerformAttack(const FGameplayTag& AttackTypeTag) const;
	void DoAttack(const FGameplayTag& AttackTypeTag);
	void ExecuteComboAttack(const FGameplayTag& AttackTypeTag);
	void ResetCombo();
	
public:
	void EnableComboWindow();
	void DisableComboWindow();
	void AttackFinished(const float ComboResetDelay);
};
