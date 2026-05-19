#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GT1MontageActionData.generated.h"

USTRUCT(BlueprintType)
struct FGT1MontageGroup
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> Animations;
};

UCLASS()
class GT1_API UGT1MontageActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="Montage Group")
	TMap<FGameplayTag, FGT1MontageGroup> MontageGroupMap;
	
public:
	TObjectPtr<UAnimMontage> GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index) const;
	
};
