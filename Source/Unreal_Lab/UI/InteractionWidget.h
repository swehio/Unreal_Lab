// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"
 
UCLASS()
class UNREAL_LAB_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPromptText(const FText& NewText);
	void SetIcon(UTexture2D* NewTexture);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PromptText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Icon;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Icon_Shadow;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> PromptHorizonBox;
};
