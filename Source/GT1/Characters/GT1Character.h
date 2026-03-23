#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GT1Character.generated.h"

UCLASS()
class GT1_API AGT1Character : public ACharacter
{
	GENERATED_BODY()

public:
	AGT1Character();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
