#include "Animations/AnimNotify_GT1AttackFinished.h"

#include "Characters/GT1Character.h"

UAnimNotify_GT1AttackFinished::UAnimNotify_GT1AttackFinished(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UAnimNotify_GT1AttackFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (AGT1Character* Character = Cast<AGT1Character>(MeshComp->GetOwner()))
	{
		Character->AttackFinished(ComboResetDelay);
	}
}
