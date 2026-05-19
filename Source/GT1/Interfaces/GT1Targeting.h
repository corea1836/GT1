#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GT1Targeting.generated.h"

UINTERFACE(MinimalAPI)
class UGT1Targeting : public UInterface
{
	GENERATED_BODY()
};

class GT1_API IGT1Targeting
{
	GENERATED_BODY()

public:
	virtual void OnTargeted(bool bTargeted) = 0;
	
	virtual bool CanBeTargeted() = 0;
};
