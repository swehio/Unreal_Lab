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

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UDialogueManagerSubsystem* DialogueManager = GI->GetSubsystem<UDialogueManagerSubsystem>())
		{
			DialogueManager->StartDialogue(DialogueData, this);
		}
	}
}
 
