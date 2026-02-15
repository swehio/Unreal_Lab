// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueInputComponent.generated.h"

class UInputMappingContext;
class UInputAction;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_LAB_API UDialogueInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueInputComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	  
	// 입력에서 호출
	void DialogueSkipOrAdvanceInput();
	void DialogueCancle();

private:
	void BindDialogueInput();

	void HandleDialogueStarted();
	void HandleDialogueEnded();
	void HandleNodeChanged(const struct FDialogueNode& Node);
	void HandleActionTriggered(FName ActionID);

	// 라인 연출(보이스/SFX) 시작/중단
	void HandleLineEventStart(const struct FDialogueLineEvent& LineEvent, AActor* NPC, AActor* Interactor);
	void HandleLineEventStop(const struct FDialogueLineEvent& LineEvent, AActor* NPC, AActor* Interactor);


private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputMappingContext> IMC_Gameplay;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Dialogue;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_DialogueAdvance;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_DialogueCancle;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> DialogueWidgetClass;

	UPROPERTY()
	TObjectPtr<class UDialogueWidget> DialogueWidget;

	UPROPERTY()
	TObjectPtr<class UDialogueManagerSubsystem> DialogueManager;

	// 스킵으로 끊기 위한 현재 보이스
	UPROPERTY()
	TObjectPtr<UAudioComponent> VoiceAudioComp;

	UPROPERTY()
	TObjectPtr<class APlayerController> CachedPlayerController;

	UPROPERTY()
	TObjectPtr<class UEnhancedInputLocalPlayerSubsystem> CachedSubsystem;
};
