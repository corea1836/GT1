#pragma once

#include "CoreMinimal.h"
#include "GT1Define.h"
#include "Blueprint/UserWidget.h"
#include "GT1PlayerHUDWidget.generated.h"

UCLASS()
class GT1_API UGT1PlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<class UGT1StatBarWidget> StaminaBarWidget;
	
public:
	UGT1PlayerHUDWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
public:
	virtual void NativeConstruct() override;
	
protected:
	void OnAttributeChanged(EGT1AttributeType AttributeType, float InValue);
};
