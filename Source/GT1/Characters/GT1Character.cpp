#include "Characters/GT1Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GT1GameplayTags.h"
#include "Blueprint/UserWidget.h"
#include "Components/GT1AttributeComponent.h"
#include "Components/GT1CombatComponent.h"
#include "Components/GT1StateComponent.h"
#include "Components/GT1TargetingComponent.h"
#include "Equipments/GT1Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/GT1Interact.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/GT1PlayerHUDWidget.h"

AGT1Character::AGT1Character()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 캐릭터가 카메라(컨트롤러) 방향을 따라가지 않도록 처리
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// 캐릭터가 입력 방향으로 회전하도록 처리
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	
	// 이동, 감속 속도
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	// 카메라 초기화
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 60.f, 30.f);
	CameraBoom->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));
	// 스프링 암은 ControllerRotation 을 따라가도록 처리
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;
	
	AttributeComponent = CreateDefaultSubobject<UGT1AttributeComponent>(TEXT("AttributeComponent"));
	StateComponent = CreateDefaultSubobject<UGT1StateComponent>(TEXT("StateComponent"));
	CombatComponent = CreateDefaultSubobject<UGT1CombatComponent>(TEXT("CombatComponent"));
	TargetingComponent = CreateDefaultSubobject<UGT1TargetingComponent>(TEXT("TargetingComponent"));
	
	/**
	 * TODO
	 * - 다이나믹 앵글
	 *   - 탐색
	 *   - 다수 적이 있는 전투
	 *   - 보스전
	 *   - 암살
	 *   - 처형
	 * - 카메라 렉
	 */
}

void AGT1Character::BeginPlay()
{
	Super::BeginPlay();
	
	// 블루프린트 클래스가 있는지 확인
	if (PlayerHUDWidgetClass)
	{
		// 블루프린트 클래스를 원본으로 실제 오브젝트 생성
		PlayerHUDWidget = CreateWidget<UGT1PlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
		if (PlayerHUDWidget)
		{
			// 생성된 오브젝트를 뷰포트에 추가
			PlayerHUDWidget->AddToViewport();
		}
	}
}

void AGT1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 스테미너 확인 디버그
	GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Cyan, FString::Printf(TEXT("Stamina : %f"), AttributeComponent->GetBaseStamina()));
	GEngine->AddOnScreenDebugMessage(2, 1.5f, FColor::Cyan, FString::Printf(TEXT("Max Walk Speed : %f"), GetCharacterMovement()->MaxWalkSpeed));
}

void AGT1Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC-> GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGT1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ThisClass::Sprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ThisClass::StopSprinting);
		
		EnhancedInputComponent->BindAction(RollingAction, ETriggerEvent::Started, this, &ThisClass::Rolling);
		
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);
		
		EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Started, this, &ThisClass::ToggleCombat);
		
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::AutoToggleCombat);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::Attack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &ThisClass::HeavyAttack);
		
		EnhancedInputComponent->BindAction(LockOnTargetAction, ETriggerEvent::Started, this, &ThisClass::LockOnTarget);
		EnhancedInputComponent->BindAction(LeftTargetAction, ETriggerEvent::Started, this, &ThisClass::LeftTarget);
		EnhancedInputComponent->BindAction(RightTargetAction, ETriggerEvent::Started, this, &ThisClass::RightTarget);
	}
}

bool AGT1Character::IsMoving() const
{
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		return MovementComponent->Velocity.Size2D() > 3.f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector;
	}
	
	return false;
}

void AGT1Character::Jump()
{
	Super::Jump();
	
	check(StateComponent);
	StateComponent->ToggleMovementInput(false);
}

void AGT1Character::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	check(StateComponent);
	StateComponent->ToggleMovementInput(true);
}

void AGT1Character::Move(const FInputActionValue& Values)
{
	check(StateComponent);
	
	if (StateComponent->MovementInputEnabled() == false)
	{
		return;
	}
	
	FVector2D MovementVector = Values.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotator(0, Rotation.Yaw, 0);
		
		const FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardVector, MovementVector.Y);
		AddMovementInput(RightVector, MovementVector.X);
	}
}

void AGT1Character::Look(const FInputActionValue& Values)
{
	if (TargetingComponent && TargetingComponent->IsLockOn())
	{
		return;
	}
	
	FVector2D LookDirection = Values.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookDirection.X);
		AddControllerPitchInput(LookDirection.Y);
	}
}

void AGT1Character::Sprinting()
{
	if (AttributeComponent->CheckHasEnoughStamina(5.f) && IsMoving())
	{
		AttributeComponent->ToggleStaminaRegeneration(false);
		
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
				
		AttributeComponent->DecreaseStamina(0.1f);
		
		bSprinting = true;
	}
	else
	{
		StopSprinting();
	}
}

void AGT1Character::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	AttributeComponent->ToggleStaminaRegeneration(true);
	bSprinting = false;
}

void AGT1Character::Rolling()
{
	check(AttributeComponent);
	check(StateComponent);
	
	if (AttributeComponent->CheckHasEnoughStamina(15.f))
	{
		AttributeComponent->ToggleStaminaRegeneration(false);
		
		StateComponent->ToggleMovementInput(false);
		
		AttributeComponent->DecreaseStamina(15.f);
		
		PlayAnimMontage(RollingMontage);
		
		StateComponent->SetState(GT1GameplayTags::Character_State_Rolling);
		
		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
}

void AGT1Character::Interact()
{
	FHitResult OutHit;
	const FVector Start = GetActorLocation();
	const FVector End = Start;
	constexpr float Radius = 100.f;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_INTERACTION));
	
	TArray<AActor*> ActorsToIgnore;
	
	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this,
		Start,
		End,
		Radius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true);
	
	if (bHit)
	{
		if (AActor* HitActor = OutHit.GetActor())
		{
			if (IGT1Interact* Interaction = Cast<IGT1Interact>(HitActor))
			{
				Interaction->Interact(this);	
			}
		}
	}
}

bool AGT1Character::CanToggleCombat() const
{
	check(StateComponent);
	
	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(GT1GameplayTags::Character_State_Attacking);
	CheckTags.AddTag(GT1GameplayTags::Character_State_Rolling);
	CheckTags.AddTag(GT1GameplayTags::Character_State_GeneralAction);
	
	return StateComponent->IsCurrentStateEqualToAny(CheckTags) == false;
}

void AGT1Character::ToggleCombat()
{
	check(CombatComponent);
	check(StateComponent);
	
	if (CombatComponent)
	{
		if (const AGT1Weapon* Weapon = CombatComponent->GetMainWeapon())
		{
			if (CanToggleCombat())
			{
				StateComponent->SetState(GT1GameplayTags::Character_State_GeneralAction);
				
				if (CombatComponent->IsCombatEnabled())
				{
					PlayAnimMontage(Weapon->GetMontageForTag(GT1GameplayTags::Character_Action_Unequip));
				}
				else
				{
					PlayAnimMontage(Weapon->GetMontageForTag(GT1GameplayTags::Character_Action_Equip));
				}
			}
		}
	}
}

void AGT1Character::AutoToggleCombat()
{
	if (CombatComponent)
	{
		if (!CombatComponent->IsCombatEnabled())
		{
			ToggleCombat();
		}
	}
}

void AGT1Character::Attack()
{
	const FGameplayTag AttackTypeTag = GetAttackPerform();
	
	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void AGT1Character::HeavyAttack()
{
	const FGameplayTag AttackTypeTag = GT1GameplayTags::Character_Attack_Heavy;
	
	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void AGT1Character::LockOnTarget()
{
	TargetingComponent->ToggleLockOn();
}

void AGT1Character::LeftTarget()
{
	TargetingComponent->SwitchingLockOnActor(ESwitchingDirection::Left);
}

void AGT1Character::RightTarget()
{
	TargetingComponent->SwitchingLockOnActor(ESwitchingDirection::Right);
}

FGameplayTag AGT1Character::GetAttackPerform() const
{
	// if (IsSprinting())
	// {
	// 	return GT1GameplayTags::Character_Attack_Running;
	// }
	return GT1GameplayTags::Character_Attack_Light;
}

bool AGT1Character::CanPerformAttack(const FGameplayTag& AttackTypeTag) const
{
	check(StateComponent);
	check(CombatComponent);
	check(AttributeComponent);
	
	if (IsValid(CombatComponent->GetMainWeapon()) == false)
	{
		return false;
	}
	
	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(GT1GameplayTags::Character_State_Rolling);
	CheckTags.AddTag(GT1GameplayTags::Character_State_GeneralAction);
	
	const float StaminaCost = CombatComponent->GetMainWeapon()->GetStaminaCost(AttackTypeTag);
	
	return StateComponent->IsCurrentStateEqualToAny(CheckTags) == false
	&& CombatComponent->IsCombatEnabled()
	&& AttributeComponent->CheckHasEnoughStamina(StaminaCost);
}

void AGT1Character::DoAttack(const FGameplayTag& AttackTypeTag)
{
	check(StateComponent);
	check(AttributeComponent);
	check(CombatComponent);
	
	if (const AGT1Weapon* Weapon = CombatComponent->GetMainWeapon())
	{
		StateComponent->SetState(GT1GameplayTags::Character_State_Attacking);
		StateComponent->ToggleMovementInput(false);
		CombatComponent->SetLastAttackType(AttackTypeTag);
		
		AttributeComponent->ToggleStaminaRegeneration(false);
		
		UAnimMontage* Montage = Weapon->GetMontageForTag(AttackTypeTag, ComboCounter);
		if (!Montage)
		{
			ComboCounter = 0;
			Montage = Weapon->GetMontageForTag(AttackTypeTag, ComboCounter);
		}
		
		PlayAnimMontage(Montage);
		
		const float StaminaCost = Weapon->GetStaminaCost(AttackTypeTag);
		AttributeComponent->DecreaseStamina(StaminaCost);
		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
	
}

void AGT1Character::ExecuteComboAttack(const FGameplayTag& AttackTypeTag)
{
	if (StateComponent->GetCurrentState() != GT1GameplayTags::Character_State_Attacking)
	{
		if (bComboSequenceRunning && bCanComboInput)
		{
			ComboCounter++;
			UE_LOG(LogTemp, Warning, TEXT("Additional input : Combo Counter %d"), ComboCounter);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(">>> ComboSequence Started <<<"));
			ResetCombo();
			bComboSequenceRunning = true;
		}
		
		DoAttack(AttackTypeTag);
		GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
	}
	else if (bCanComboInput)
	{
		bSavedComboInput = true; 
		
		
		
	}
}

void AGT1Character::ResetCombo()
{
	UE_LOG(LogTemp, Warning, TEXT("Combo Reset"));
	
	bComboSequenceRunning = false;
	bCanComboInput = false;
	bSavedComboInput = false;
	ComboCounter = 0;
}

void AGT1Character::EnableComboWindow()
{
	bCanComboInput = true;
	UE_LOG(LogTemp, Warning, TEXT("Combo Window Opened : Combo Counter %d"), ComboCounter);
}

void AGT1Character::DisableComboWindow()
{
	check(CombatComponent);
	
	bCanComboInput = false;
	
	if (bSavedComboInput)
	{
		bSavedComboInput = false;
		ComboCounter++;
		UE_LOG(LogTemp, Warning, TEXT("Combo Window Closed : Advancing to next combo = %d"), ComboCounter);
		DoAttack(CombatComponent->GetLastAttackType());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Combo Window Closed : No input received"));
	}
	
}

void AGT1Character::AttackFinished(const float ComboResetDelay)
{
	UE_LOG(LogTemp, Warning, TEXT("AttackFinished"));
	if (StateComponent)
	{
		StateComponent->ToggleMovementInput(true);
	}
	GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ThisClass::ResetCombo, ComboResetDelay, false);
}

