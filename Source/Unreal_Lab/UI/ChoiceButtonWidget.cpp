// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/ChoiceButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UChoiceButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();


}

void UChoiceButtonWidget::Init(int8 InIndex, const FText& InText)
{
	ChoiceIndex = InIndex;
	if (ChoiceText)
	{
		ChoiceText->SetText(InText);
	} 

	if (ChoiceButton)
	{
		ChoiceButton->OnClicked.AddDynamic(
			this, &UChoiceButtonWidget::HandleClicked
		);
	}
}

void UChoiceButtonWidget::HandleClicked()
{
	OnChoiceClicked.Broadcast(ChoiceIndex);
	UE_LOG(LogTemp, Warning, TEXT("Widget instance: %p"), this);
}
