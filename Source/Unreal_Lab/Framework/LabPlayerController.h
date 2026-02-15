#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h" 
#include "Data/DialogueDataAsset.h"
#include "LabPlayerController.generated.h" 
 
class IInteractable;
class UInputAction;
class UUserWidget;

UCLASS()
class UNREAL_LAB_API ALabPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALabPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
	void UpdateInteractionUI();

	//// 입력에서 호출
	//void DialogueSkipOrAdvanceInput();
	//void DialogueCancle();

protected: 
	virtual void OnPossess(APawn* InPawn) override; 

private:
	UFUNCTION()
	void HandleInteractionTargetChanged(UObject* NewTarget);

	//void HandleDialogueStarted();
	//void HandleDialogueEnded();
	//void HandleNodeChanged(const struct FDialogueNode& Node);
	//void HandleActionTriggered(FName ActionID);

	//// 라인 연출(보이스/SFX) 시작/중단
	//void HandleLineEventStart(const struct FDialogueLineEvent& LineEvent, AActor* NPC, AActor* Interactor);
	//void HandleLineEventStop(const struct FDialogueLineEvent& LineEvent, AActor* NPC, AActor* Interactor);
	 
	void Move(const FInputActionValue& Value); 
	void Look(const FInputActionValue& Value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value); 
	void StartCrouch(const FInputActionValue& Value);
	void StopCrouch(const FInputActionValue& Value);
	void OnInteractPressed(const FInputActionValue& Value);


protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDialogueInputComponent> DialogueInputComp;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<class UInputMappingContext> IMC_Gameplay;

	//UPROPERTY(EditAnywhere, Category = "Input")
	//TObjectPtr<class UInputMappingContext> IMC_Dialogue;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Move; 

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Jump;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Sprint;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Crouch;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Interact;

	//UPROPERTY(EditAnywhere, Category = "Input")
	//TObjectPtr<UInputAction> IA_DialogueAdvance;

	//UPROPERTY(EditAnywhere, Category = "Input")
	//TObjectPtr<UInputAction> IA_DialogueCancle;

	UPROPERTY()
	TObjectPtr<class UEnhancedInputLocalPlayerSubsystem> CachedSubsystem;

	UPROPERTY()
	TObjectPtr<class ALabPlayerCharacter> CachedPlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category= "UI")
	TSubclassOf<UUserWidget> InteractionUIClass;

	UPROPERTY()
	TObjectPtr<class UInteractionWidget> InteractionUI;

	TObjectPtr<UObject> CurrentInteractTarget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> PlayerHUDWidget;

	//UPROPERTY(EditDefaultsOnly, Category = "UI")
	//TSubclassOf<UUserWidget> DialogueWidgetClass;

	//UPROPERTY()
	//TObjectPtr<class UDialogueWidget> DialogueWidget;

	//UPROPERTY()
	//TObjectPtr<class UDialogueManagerSubsystem> DialogueManager;

	// 스킵으로 끊기 위한 현재 보이스
	//UPROPERTY()
	//TObjectPtr<UAudioComponent> VoiceAudioComp;
};
