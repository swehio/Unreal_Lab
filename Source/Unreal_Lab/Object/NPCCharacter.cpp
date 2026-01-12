// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/NPCCharacter.h"
#include "Subsystem/DialogueManagerSubsystem.h"
#include "Engine/GameInstance.h"
#include "NPCCharacter.h"

// Sets default values
ANPCCharacter::ANPCCharacter()
{ 
	PrimaryActorTick.bCanEverTick = false; 
}

void ANPCCharacter::Interact_Implementation(AActor* Interactor)
{
	if (!DialogueData) return;

	if(DialogueManager&&!DialogueManager->IsRunning()) DialogueManager->StartDialogue(DialogueData, Interactor, this); 
}

void ANPCCharacter::BeginPlay()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		DialogueManager = GI->GetSubsystem<UDialogueManagerSubsystem>();
	}

	if (DialogueManager)
	{
		DialogueManager->OnDialogueLineEventStart.AddUObject(this, &ANPCCharacter::HandleLineEventStart);
		DialogueManager->OnDialogueLineEventStop.AddUObject(this, &ANPCCharacter::HandleLineEventStop);
	}
}
 
void ANPCCharacter::HandleLineEventStart(const FDialogueLineEvent& LineEvent, AActor* InNPC, AActor* InInteractor)
{
	// 이 이벤트가 "나"를 대상으로 한 대화가 아니면 무시
	if (InNPC != this) return;

	if (LineEvent.AnimMontage)
	{
		//PlayAnimMontage(LineEvent.AnimMontage);
	}
}

void ANPCCharacter::HandleLineEventStop(const FDialogueLineEvent& LineEvent, AActor* InNPC, AActor* InInteractor)
{
	if (InNPC != this) return;

	if (LineEvent.AnimMontage)
	{
		//StopAnimMontage(LineEvent.AnimMontage);
	}
}