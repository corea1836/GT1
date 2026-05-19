#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_GT1WpCollision.generated.h"

UCLASS()
class GT1_API UAnimNotifyState_GT1WpCollision : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_GT1WpCollision(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
