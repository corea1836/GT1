#include "Characters/GT1Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/GT1AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

void AGT1Character::Move(const FInputActionValue& Values)
{
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
}

