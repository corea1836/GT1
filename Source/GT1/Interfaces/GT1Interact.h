#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GT1Interact.generated.h"

UINTERFACE(MinimalAPI)
class UGT1Interact : public UInterface
{
	GENERATED_BODY()
};

class GT1_API IGT1Interact
{
	GENERATED_BODY()

public:
	virtual void Interact(AActor* Interactor) = 0;
};
