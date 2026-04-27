#include "Animations/AnimNotifyState_GT1ComboWindow.h"

#include "Characters/GT1Character.h"

UAnimNotifyState_GT1ComboWindow::UAnimNotifyState_GT1ComboWindow(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UAnimNotifyState_GT1ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (AGT1Character* Character = Cast<AGT1Character>(MeshComp->GetOwner()))
	{
		Character->EnableComboWindow();
	}
}

void UAnimNotifyState_GT1ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (AGT1Character* Character = Cast<AGT1Character>(MeshComp->GetOwner()))
	{
		Character->DisableComboWindow();
	}
}
