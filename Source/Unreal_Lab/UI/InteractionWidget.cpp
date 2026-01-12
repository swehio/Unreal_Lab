// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"

void UInteractionWidget::SetPromptText(const FText& NewText)
{
	if (PromptText)
	{
		if (NewText.IsEmpty())
		{
			PromptHorizonBox->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			PromptHorizonBox->SetVisibility(ESlateVisibility::HitTestInvisible);
			PromptText->SetText(NewText); 
		}
	} 
}

void UInteractionWidget::SetIcon(UTexture2D* NewTexture)
{
	if (Icon)
	{
		Icon->SetBrushFromTexture(NewTexture);
		Icon_Shadow->SetBrushFromTexture(NewTexture);
	}
}
