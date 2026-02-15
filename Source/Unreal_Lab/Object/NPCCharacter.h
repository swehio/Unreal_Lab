// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "Object/InteractableActor.h"

#include "NPCCharacter.generated.h"
  
//class UDialogueDataAsset;
//class UDialogueManagerSubsystem;

UCLASS()
class UNREAL_LAB_API ANPCCharacter : public AInteractableActor
{
	GENERATED_BODY()
	
public:	 
	ANPCCharacter();

public: 
	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDialogueTargetComponent> DialogueTargetComp;

//protected:
//	virtual void BeginPlay() override;

//private:
//	void HandleLineEventStart(const struct FDialogueLineEvent& LineEvent, AActor* NPC, AActor* Interactor);
//	void HandleLineEventStop(const struct FDialogueLineEvent& LineEvent, AActor* NPC, AActor* Interactor);
//
//protected:
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
//	TObjectPtr<class UDialogueDataAsset> DialogueData;
//private:
//	UPROPERTY()
//	TObjectPtr<UDialogueManagerSubsystem> DialogueManager;

};
