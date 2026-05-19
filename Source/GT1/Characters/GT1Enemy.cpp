#include "Characters/GT1Enemy.h"

#include "GT1GameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "Components/GT1AttributeComponent.h"
#include "Components/GT1StateComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"

AGT1Enemy::AGT1Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	TargetingSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TargetingSphereComponent"));
	TargetingSphereComponent->SetupAttachment(GetRootComponent());
	TargetingSphereComponent->SetCollisionObjectType(COLLISION_OBJECT_TARGETING);
	TargetingSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetingSphereComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	LockonWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidgetComponent"));
	LockonWidgetComponent->SetupAttachment(GetRootComponent());
	LockonWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	
	
	LockonWidgetComponent->SetDrawSize(FVector2d(50.f, 50.f));
	LockonWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	LockonWidgetComponent->SetVisibility(false);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	AttributeComponent = CreateDefaultSubobject<UGT1AttributeComponent>(TEXT("AttributeComponent"));
	StateComponent = CreateDefaultSubobject<UGT1StateComponent>(TEXT("StateComponent"));
	
	AttributeComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);
}

void AGT1Enemy::BeginPlay()
{
	Super::BeginPlay();
}

void AGT1Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AGT1Enemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (AttributeComponent)
	{
		AttributeComponent->TakeDamageAmount(ActualDamage);
		GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Cyan, FString::Printf(TEXT("Damaged : %f"), ActualDamage));
	}
	
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		
		FVector ShotDirection = PointDamageEvent->ShotDirection;
		
		FVector ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;
		
		FVector ImpactDirection = PointDamageEvent->HitInfo.ImpactNormal;
		
		FVector HitLocation = PointDamageEvent->HitInfo.Location;
		
		ImpactEffect(ImpactPoint);
		
		HitReaction(EventInstigator->GetPawn());
	}
	
	return ActualDamage;
}

void AGT1Enemy::OnDeath()
{
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	if (USkeletalMeshComponent* MeshComponent = GetMesh())
	{
		MeshComponent->SetCollisionProfileName("Ragdoll");
		MeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		MeshComponent->SetSimulatePhysics(true);
	}
}

void AGT1Enemy::ImpactEffect(const FVector& Location)
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location);
	}
	
	if (ImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Location);
	}
}

void AGT1Enemy::HitReaction(const AActor* Attacker)
{
	if (UAnimMontage* HitReactAnimMontage = GetHitReactAnimation(Attacker))
	{
		float DelaySeconds = PlayAnimMontage(HitReactAnimMontage);
	}
}

TObjectPtr<UAnimMontage> AGT1Enemy::GetHitReactAnimation(const AActor* Attacker) const
{
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Attacker->GetActorLocation());
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), LookAtRotation);
	
	const float DeltaZ = DeltaRotation.Yaw;
	
	EHitDirection HitDirection = EHitDirection::Front;
	
	if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -45.f, 45.f))
	{
		HitDirection = EHitDirection::Front;
		UE_LOG(LogTemp, Log, TEXT("Front"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -135.f, -45.f))
	{
		HitDirection = EHitDirection::Left;
		UE_LOG(LogTemp, Log, TEXT("Left"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 135.f, 180.f)
		|| UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -180.f, -135.f))
	{
		HitDirection = EHitDirection::Back;
		UE_LOG(LogTemp, Log, TEXT("Back"));
	}	
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 45.f, 135))
	{
		HitDirection = EHitDirection::Right;
		UE_LOG(LogTemp, Log, TEXT("Right"));
	}
	
	UAnimMontage* SelectedMontage = nullptr;
	switch (HitDirection)
	{
	case EHitDirection::Front:
		SelectedMontage = HitReactAnimFront;
		break;
	case EHitDirection::Back:
		SelectedMontage = HitReactAnimBack;
		break;
	case EHitDirection::Left:
		SelectedMontage = HitReactAnimLeft;
		break;
	case EHitDirection::Right:
		SelectedMontage = HitReactAnimRight;
		break;
	}
	
	return SelectedMontage;
	
}

void AGT1Enemy::OnTargeted(bool bTargeted)
{
	if (LockonWidgetComponent)
	{
		LockonWidgetComponent->SetVisibility(bTargeted);
	}
}

bool AGT1Enemy::CanBeTargeted()
{
	if (!StateComponent)
	{
		return false;
	}
	
	FGameplayTagContainer TagCheck;
	TagCheck.AddTag(GT1GameplayTags::Character_State_Death);
	
	return StateComponent->IsCurrentStateEqualToAny(TagCheck) == false;
}

