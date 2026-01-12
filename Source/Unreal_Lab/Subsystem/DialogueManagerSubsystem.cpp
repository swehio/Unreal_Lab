// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DialogueManagerSubsystem.h"
#include "Data/DialogueDataAsset.h"

void UDialogueManagerSubsystem::StartDialogue(UDialogueDataAsset* InData, AActor* InInteractor, AActor* InNPC)
{
	if (!InData) return;

	if (bRunning)
	{
		EndDialogue();
	}

	Data = InData;
	Interactor = InInteractor;
	NPC = InNPC;

	bRunning = true;
	CurrentID = NAME_None;
	CurrentNodePtr = nullptr;
	CurrentLineEvent = FDialogueLineEvent();

	OnDialogueStarted.Broadcast();

	MoveToNode(Data->StartDialogueID);
} 

void UDialogueManagerSubsystem::EndDialogue()
{
	if (!bRunning) return;

	// 대화 종료 시 연출 정리
	StopCurrentLineEvent();

	bRunning = false;
	Data = nullptr;
	Interactor = nullptr;
	NPC = nullptr;
	CurrentNodePtr = nullptr;
	CurrentID = NAME_None;
	CurrentLineEvent = FDialogueLineEvent();

	OnDialogueEnded.Broadcast();
}

void UDialogueManagerSubsystem::MoveToNode(FName DialogueID)
{
	if (!Data || DialogueID.IsNone())
	{
		EndDialogue();
		return;
	}

	const FDialogueNode* Found = Data->DialogueNodes.Find(DialogueID);
	if (!Found)
	{
		EndDialogue();
		return;
	}

	// 노드 이동 전 기존 라인 연출 중단(원하면 유지 정책도 가능하지만 기본은 중단)
	StopCurrentLineEvent();

	CurrentID = DialogueID;
	CurrentNodePtr = Found;

	// 라인 이벤트 캐시
	CurrentLineEvent = CurrentNodePtr->LineEvent;

	OnDialogueNodeChanged.Broadcast(*CurrentNodePtr);

	// 라인 시작 연출(보이스/몽타주 등)
	if (CurrentLineEvent.bPlayAtLineStart)
	{
		OnDialogueLineEventStart.Broadcast(CurrentLineEvent, NPC.Get(), Interactor.Get());
	}
}

void UDialogueManagerSubsystem::Advance()
{
	if (!bRunning || !CurrentNodePtr) return;

	// 선택지가 있으면 Advance로 넘어가지 않음
	if (CurrentNodePtr->Choices.Num() > 0)
	{
		return;
	}

	if (!CurrentNodePtr->NextDialogueID.IsNone())
	{
		MoveToNode(CurrentNodePtr->NextDialogueID);
	}
	else
	{
		EndDialogue();
	}
}

void UDialogueManagerSubsystem::SelectChoice(int32 ChoiceIndex)
{
	if (!bRunning || !CurrentNodePtr) return;
	if (CurrentNodePtr->Choices.Num() == 0) return;
	if (!CurrentNodePtr->Choices.IsValidIndex(ChoiceIndex)) return;

	const FDialogueChoice& Choice = CurrentNodePtr->Choices[ChoiceIndex];

	ExecuteActionIfAny(Choice.ActionID);

	if (!Choice.NextDialogueID.IsNone())
	{
		MoveToNode(Choice.NextDialogueID);
	}
	else
	{
		EndDialogue();
	}
}

void UDialogueManagerSubsystem::ExecuteActionIfAny(FName ActionID)
{
	if (ActionID.IsNone()) return;
	OnDialogueActionTriggered.Broadcast(ActionID);
}

void UDialogueManagerSubsystem::StopCurrentLineEvent()
{
	if (!bRunning) return;

	// 캐시된 라인 이벤트가 “실제로 뭔가”가 있을 때만 중단 브로드캐스트
	if (CurrentLineEvent.VoiceSound || CurrentLineEvent.SFX || CurrentLineEvent.AnimMontage)
	{
		OnDialogueLineEventStop.Broadcast(CurrentLineEvent, NPC.Get(), Interactor.Get());
	}

	CurrentLineEvent = FDialogueLineEvent();
}