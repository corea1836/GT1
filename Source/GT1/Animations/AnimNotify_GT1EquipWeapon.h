#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_GT1EquipWeapon.generated.h"

UCLASS()
class GT1_API UAnimNotify_GT1EquipWeapon : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag MontageActionTag;
	
public:
	UAnimNotify_GT1EquipWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
