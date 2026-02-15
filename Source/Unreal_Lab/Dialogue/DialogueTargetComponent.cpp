// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/DialogueTargetComponent.h"
#include "Subsystem/DialogueManagerSubsystem.h"
#include "Engine/GameInstance.h"

// Sets default values for this component's properties
UDialogueTargetComponent::UDialogueTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UDialogueTargetComponent::InteractDialogue(AActor* Interactor)
{ 
	if (!DialogueManager||!DialogueData) return;

	if (DialogueManager && !DialogueManager->IsRunning()) DialogueManager->StartDialogue(DialogueData, Interactor, Owner);
}

void UDialogueTargetComponent::BeginPlay()
{
	Owner = GetOwner();
	if (!Owner) return;
	if (UGameInstance* GI = Owner->GetGameInstance())
	{
		DialogueManager = GI->GetSubsystem<UDialogueManagerSubsystem>();
	}

	if (DialogueManager)
	{
		DialogueManager->OnDialogueLineEventStart.AddUObject(this, &UDialogueTargetComponent::HandleLineEventStart);
		DialogueManager->OnDialogueLineEventStop.AddUObject(this, &UDialogueTargetComponent::HandleLineEventStop);
	}
}

void UDialogueTargetComponent::HandleLineEventStart(const FDialogueLineEvent& LineEvent, AActor* InNPC, AActor* InInteractor)
{
	if (!Owner) return;
	// 이 이벤트가 "나"를 대상으로 한 대화가 아니면 무시
	if (InNPC != Owner) return;

	if (LineEvent.AnimMontage)
	{
		//PlayAnimMontage(LineEvent.AnimMontage);
	}
}

void UDialogueTargetComponent::HandleLineEventStop(const FDialogueLineEvent& LineEvent, AActor* InNPC, AActor* InInteractor)
{
	if (!Owner) return;
	if (InNPC != Owner) return;

	if (LineEvent.AnimMontage)
	{
		//StopAnimMontage(LineEvent.AnimMontage);
	}
}
