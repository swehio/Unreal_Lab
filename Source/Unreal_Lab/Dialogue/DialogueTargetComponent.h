// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueTargetComponent.generated.h"

class UDialogueDataAsset;
class UDialogueManagerSubsystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_LAB_API UDialogueTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueTargetComponent();

	void InteractDialogue(AActor* Interactor);

protected:
	virtual void BeginPlay() override;

private:
	void HandleLineEventStart(const struct FDialogueLineEvent& LineEvent, AActor* NPC, AActor* Interactor);
	void HandleLineEventStop(const struct FDialogueLineEvent& LineEvent, AActor* NPC, AActor* Interactor);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TObjectPtr<UDialogueDataAsset> DialogueData;
private:
	UPROPERTY()
	TObjectPtr<UDialogueManagerSubsystem> DialogueManager;

	UPROPERTY()
	TObjectPtr<AActor> Owner;

public:
	UDialogueDataAsset* GetDialogueData() const { return DialogueData; }
};
