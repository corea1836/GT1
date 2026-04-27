#include "Data/GT1MontageActionData.h"


TObjectPtr<UAnimMontage> UGT1MontageActionData::GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index) const
{
	if (MontageGroupMap.Contains(GroupTag))
	{
		const FGT1MontageGroup& MontageGroup = MontageGroupMap[GroupTag];
		
		if (MontageGroup.Animations.Num() > 0 && MontageGroup.Animations.Num() > Index)
		{
			return MontageGroup.Animations[Index];
		}
	}
	
	return nullptr;
}
