#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/GT1Targeting.h"
#include "GT1Enemy.generated.h"

UCLASS()
class GT1_API AGT1Enemy : public ACharacter, public IGT1Targeting
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> TargetingSphereComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGT1AttributeComponent> AttributeComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGT1StateComponent> StateComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWidgetComponent> LockonWidgetComponent;
	
protected:
	UPROPERTY(EditAnywhere, Category="Effect")
	TObjectPtr<USoundCue> ImpactSound;
	
	UPROPERTY(EditAnywhere, Category="Effect")
	TObjectPtr<UParticleSystem> ImpactParticle;

protected:
	UPROPERTY(EditAnywhere, Category="Montage | HitReact")
	TObjectPtr<UAnimMontage> HitReactAnimFront;
	
	UPROPERTY(EditAnywhere, Category="Montage | HitReact")
	TObjectPtr<UAnimMontage> HitReactAnimBack;
	
	UPROPERTY(EditAnywhere, Category="Montage | HitReact")
	TObjectPtr<UAnimMontage> HitReactAnimLeft;
	
	UPROPERTY(EditAnywhere, Category="Montage | HitReact")
	TObjectPtr<UAnimMontage> HitReactAnimRight;
	
public:
	AGT1Enemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
public:
	virtual void OnDeath();
	
protected:
	void ImpactEffect(const FVector& Location);
	
	void HitReaction(const AActor* Attacker);
	
	TObjectPtr<UAnimMontage> GetHitReactAnimation(const AActor* Attacker) const;
	
public:
	virtual void OnTargeted(bool bTargeted) override;
	
	virtual bool CanBeTargeted() override;
};
