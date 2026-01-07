// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UI/ChoiceButtonWidget.h"
#include "Subsystem/DialogueManagerSubsystem.h"

void UDialogueWidget::Bind(UDialogueManagerSubsystem* Manager)
{
	BoundManager = Manager;
	if (BoundManager)
	{
		BoundManager->OnDialogueInfoChanged.AddDynamic(this, &UDialogueWidget::OnInfoChanged);
	}
}

void UDialogueWidget::OnInfoChanged(const FDialogueInfo& Info)
{
	StartTypewriter(Info.DialogueText);
	if (NameText) NameText->SetText(Info.SpearkerName);
	PendingChoices = Info.Choices;
}

void UDialogueWidget::StartTypewriter(const FText& Text)
{
	FullText = Text.ToString();
	CurrentText.Empty();
	CharIndex = 0;
	bTyping = true;

	GetWorld()->GetTimerManager().SetTimer(TypingTimer, this, &UDialogueWidget::TypeNextChar, TypingInterval, true);
}

void UDialogueWidget::TypeNextChar()
{
	if (CharIndex >= FullText.Len())
	{
		FinishTypewriter();
		return;
	}

	CurrentText.AppendChar(FullText[CharIndex++]);
	if(DialogueText) DialogueText->SetText(FText::FromString(CurrentText));
}

void UDialogueWidget::FinishTypewriter()
{
	bTyping = false;
	GetWorld()->GetTimerManager().ClearTimer(TypingTimer);
	CurrentText = FullText;
	if (DialogueText) DialogueText->SetText(FText::FromString(FullText));
	CreateChoiceButtons(PendingChoices);
}

void UDialogueWidget::OnAdvanceInput()
{
	if (bTyping)
	{
		FinishTypewriter();
	} 
}

void UDialogueWidget::OnChiceClicked(int32 Index)
{
	if (BoundManager)
	{
		BoundManager->EndDialogue();
	}
}

void UDialogueWidget::CreateChoiceButtons(const TArray<FDialogueChoice>& Choices)
{
	ChoiceContainer->ClearChildren();

	for (int8 i = 0; i < Choices.Num(); i++)
	{
		UChoiceButtonWidget* Button = CreateWidget<UChoiceButtonWidget>(this, ChoiceButtonClass);

		Button->Init(i, Choices[i].ChoiceText);

		Button->OnChoiceClicked.AddDynamic(this, &UDialogueWidget::HandleChoiceSelected);

		ChoiceContainer->AddChild(Button);
	}
}

void UDialogueWidget::HandleChoiceSelected(int32 Index)
{
	if (BoundManager)
	{
		BoundManager->SelectChoice(Index);
		UE_LOG(LogTemp, Warning, TEXT("Select Index : %d"), Index); 
	}

	ChoiceContainer->ClearChildren();
} 
