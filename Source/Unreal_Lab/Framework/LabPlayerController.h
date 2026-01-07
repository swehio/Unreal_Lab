#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h" 
#include "Data/DialogueDataAsset.h"
#include "LabPlayerController.generated.h" 
 
class IInteractable;

UCLASS()
class UNREAL_LAB_API ALabPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

protected: 
	virtual void OnPossess(APawn* InPawn) override;

private:
	UFUNCTION()
	void HandleInteractionTargetChanged(UObject* NewTarget);

	UFUNCTION()
	void HandleDialogueStarted(const FDialogueInfo& Info);

	UFUNCTION()
	void HandleDialogueEnded();

	void Move(const FInputActionValue& Value); 
	void Look(const FInputActionValue& Value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value); 
	void StartCrouch(const FInputActionValue& Value);
	void StopCrouch(const FInputActionValue& Value);
	void OnInteractPressed(const FInputActionValue& Value);
	void UpdateInteractionUI();

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<class UInputMappingContext> PlayerIMC;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> IA_Move; 

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> IA_Look;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> IA_Jump;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> IA_Sprint;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> IA_Crouch;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> IA_Interact;

	UPROPERTY()
	TObjectPtr<class ALabPlayerCharacter> CachedPlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category= "UI")
	TSubclassOf<class UUserWidget> InteractionUIClass;

	UPROPERTY()
	TObjectPtr<class UInteractionWidget> InteractionUI;

	TObjectPtr<UObject> CurrentInteractTarget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> DialogueWidgetClass;

	UPROPERTY()
	TObjectPtr<class UDialogueWidget> DialogueWidget;

	UPROPERTY()
	TObjectPtr<class UDialogueManagerSubsystem> DialogueManager;

};
