// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UI/ChoiceButtonWidget.h"
#include "Subsystem/DialogueManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HideAll();
}

void UDialogueWidget::HideAll()
{
	SetVisibility(ESlateVisibility::Hidden);

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TypingTimer);
	}

	if (ChoiceList)
	{
		ChoiceList->ClearChildren();
	}

	PlayState = EDialoguePlayState::Hidden;
	FullLine.Empty();
	CurrentCharIndex = 0;
}

void UDialogueWidget::ShowNode(const FDialogueNode& Node)
{
	CachedNode = Node;

	SetVisibility(ESlateVisibility::Visible);

	if (SpeakerText) SpeakerText->SetText(Node.SpeakerName);
	if (ChoiceList) ChoiceList->ClearChildren();

	StartTyping(Node.DialogueText);

	// 타이핑 중에는 선택지 아직 미표시
	PlayState = EDialoguePlayState::Typing;
}

void UDialogueWidget::StartTyping(const FText& FullText)
{
	if (!GetWorld() || !LineText) return;

	GetWorld()->GetTimerManager().ClearTimer(TypingTimer);

	FullLine = FullText.ToString();
	CurrentCharIndex = 0;
	LineText->SetText(FText::GetEmpty());

	GetWorld()->GetTimerManager().SetTimer(
		TypingTimer,
		this,
		&UDialogueWidget::TickTyping,
		TypingInterval,
		true
	);
}

void UDialogueWidget::TickTyping()
{
	if (!GetWorld() || !LineText) return;

	if (CurrentCharIndex >= FullLine.Len())
	{
		GetWorld()->GetTimerManager().ClearTimer(TypingTimer);

		// 타이핑 종료
		PlayState = EDialoguePlayState::PlayingLineEvent;

		// 선택지 있으면 여기서 표시하고 선택 대기
		if (CachedNode.Choices.Num() > 0)
		{
			BuildChoices(CachedNode.Choices);
			PlayState = EDialoguePlayState::WaitingForChoice;
			return;
		}

		// 자동 진행이면 딜레이 후 Advance
		if (CachedNode.bAutoAdvance && Manager)
		{
			FTimerHandle Tmp;
			GetWorld()->GetTimerManager().SetTimer(Tmp, [this]()
				{
					if (Manager)
					{
						Manager->Advance();
					}
				}, CachedNode.AutoAdvanceDelay, false);

			return;
		}

		// 수동 입력 대기
		PlayState = EDialoguePlayState::WaitingForInput;
		return;
	}

	CurrentCharIndex++;
	const FString Partial = FullLine.Left(CurrentCharIndex);
	LineText->SetText(FText::FromString(Partial));

	// 타이핑 효과음(너무 잦으면 피로하니 필요하면 Interval을 키우거나 랜덤으로)
	if (TypingSFX)
	{
		UGameplayStatics::PlaySound2D(this, TypingSFX);
	}
}

void UDialogueWidget::FinishTypingImmediately()
{
	if (!GetWorld() || !LineText) return;

	GetWorld()->GetTimerManager().ClearTimer(TypingTimer);

	LineText->SetText(FText::FromString(FullLine));

	// 타이핑 스킵 후: 선택지 있으면 표시 / 없으면 대기(또는 자동진행)
	if (CachedNode.Choices.Num() > 0)
	{
		BuildChoices(CachedNode.Choices);
		PlayState = EDialoguePlayState::WaitingForChoice;
		return;
	}

	if (CachedNode.bAutoAdvance && Manager)
	{
		FTimerHandle Tmp;
		GetWorld()->GetTimerManager().SetTimer(Tmp, [this]()
			{
				if (Manager) Manager->Advance();
			}, CachedNode.AutoAdvanceDelay, false);

		PlayState = EDialoguePlayState::PlayingLineEvent;
		return;
	}

	PlayState = EDialoguePlayState::WaitingForInput;
}

void UDialogueWidget::StopLineEventsRequest()
{
	// 문장 연출 스킵(보이스/몽타주 중단)
	if (Manager)
	{
		Manager->StopCurrentLineEvent();
	}
}

void UDialogueWidget::OnSkipOrAdvanceInput()
{
	// 노드 스킵 금지면 무시(컷신용)
	if (!CachedNode.bAllowSkip)
	{
		// 단, 타이핑 중 스킵도 막고 싶으면 여기서 return
		// 지금은 "완전 금지" 정책
		return;
	}

	switch (PlayState)
	{
	case EDialoguePlayState::Typing:
		// 1) 타이핑 스킵(문장 완성)
		FinishTypingImmediately();
		break;

	case EDialoguePlayState::PlayingLineEvent:
		// 2) 연출 스킵(보이스/몽타주 중단) -> 입력 대기 상태로
		StopLineEventsRequest();
		PlayState = EDialoguePlayState::WaitingForInput;
		break;

	case EDialoguePlayState::WaitingForInput:
		// 3) 다음 문장
		if (Manager) Manager->Advance();
		break;

	case EDialoguePlayState::WaitingForChoice:
		// 선택지는 스킵 불가(명시적 선택)
		break;

	default:
		break;
	}
}

void UDialogueWidget::BuildChoices(const TArray<FDialogueChoice>& Choices)
{
	if (!ChoiceList || !ChoiceButtonClass) return;

	ChoiceList->ClearChildren();

	for (int32 i = 0; i < Choices.Num(); i++)
	{
		UChoiceButtonWidget* Btn = CreateWidget<UChoiceButtonWidget>(this, ChoiceButtonClass);
		if (!Btn) continue;

		Btn->Init(i, Choices[i].ChoiceText);
		Btn->OnChoiceClicked.AddDynamic(this, &UDialogueWidget::HandleChoiceClicked);

		ChoiceList->AddChild(Btn);
	}
}

void UDialogueWidget::HandleChoiceClicked(int32 ChoiceIndex)
{
	if (Manager)
	{
		Manager->SelectChoice(ChoiceIndex);
	}
}