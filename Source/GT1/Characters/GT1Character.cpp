#include "Characters/GT1Character.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AGT1Character::AGT1Character()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 카메라 초기화
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 60.f, 30.f);
	CameraBoom->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	
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
	
}

void AGT1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		
	}
}

void AGT1Character::Move(const FInputActionValue& Values)
{
}

void AGT1Character::Look(const FInputActionValue& Values)
{
}

