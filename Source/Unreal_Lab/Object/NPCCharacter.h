// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "Object/InteractableActor.h"

#include "NPCCharacter.generated.h"
  
UCLASS()
class UNREAL_LAB_API ANPCCharacter : public AInteractableActor
{
	GENERATED_BODY()
	
public:	 
	ANPCCharacter();

public: 
	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TObjectPtr<class UDialogueDataAsset> DialogueData;
};
