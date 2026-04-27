#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_GT1AttackFinished.generated.h"

UCLASS(meta=(DisplayName = "Attack Finished"))
class GT1_API UAnimNotify_GT1AttackFinished : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	float ComboResetDelay = 1.f;
	
public:
	UAnimNotify_GT1AttackFinished(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
