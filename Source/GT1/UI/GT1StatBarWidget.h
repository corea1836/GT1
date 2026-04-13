#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GT1StatBarWidget.generated.h"

UCLASS()
class GT1_API UGT1StatBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<class UProgressBar> StatBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StatBar")
	FLinearColor FillColorAndOpacity = FLinearColor::Red;
	
public:
	virtual void NativePreConstruct() override;
	
public:
	UGT1StatBarWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	void SetRatio(float Ratio) const;
};
