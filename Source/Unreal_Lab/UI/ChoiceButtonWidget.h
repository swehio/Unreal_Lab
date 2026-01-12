// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChoiceButtonWidget.generated.h"
 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChoiceClicked, int32, ChoiceIndex);

UCLASS()
class UNREAL_LAB_API UChoiceButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void Init(int8 InIndex, const FText& InText);

	UPROPERTY(BlueprintAssignable)
	FOnChoiceClicked OnChoiceClicked;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* ChoiceButton;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChoiceText;
	UFUNCTION()
	void HandleClicked();

private:
	int8 ChoiceIndex;
	
};
