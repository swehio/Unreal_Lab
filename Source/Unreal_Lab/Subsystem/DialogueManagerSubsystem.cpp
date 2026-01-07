// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DialogueManagerSubsystem.h"

void UDialogueManagerSubsystem::StartDialogue(UDialogueDataAsset* InData, AActor* Speaker)
{
	if (!InData) return;

	CurrentDialogue = InData;
	CurrentDialogueID = InData->StartDialogueID;
	CurrentSpeaker = Speaker;

	if (const FDialogueInfo* Info = GetCurrentInfo())
	{
		OnDialogueInfoChanged.Broadcast(*Info);
		OnDialogueStarted.Broadcast();
	} 
}

void UDialogueManagerSubsystem::SelectChoice(int32 Index)
{
	const FDialogueInfo* Info = GetCurrentInfo();
	if (!Info || !Info->Choices.IsValidIndex(Index)) return;

	const FDialogueChoice& Choice = Info->Choices[Index];

	if (Choice.NextDialogueID.IsNone())
	{
		EndDialogue();
		return;
	}

	CurrentDialogueID = Choice.NextDialogueID;;

	if (const FDialogueInfo* Next = GetCurrentInfo())
	{
		OnDialogueInfoChanged.Broadcast(*Next);
	}
}

void UDialogueManagerSubsystem::EndDialogue()
{
	CurrentDialogue = nullptr;
	CurrentDialogueID = NAME_None;
	CurrentSpeaker = nullptr;
	OnDialogueEnded.Broadcast();
}

const FDialogueInfo* UDialogueManagerSubsystem::GetCurrentInfo() const
{
	if (!CurrentDialogue) return nullptr;
	return CurrentDialogue->DialogueMap.Find(CurrentDialogueID);
}
