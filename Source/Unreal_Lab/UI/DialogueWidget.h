// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/DialogueDataAsset.h"
#include "DialogueWidget.generated.h"


UCLASS()
class UNREAL_LAB_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Bind(class UDialogueManagerSubsystem* Manager);

protected:
	UFUNCTION()
	void OnInfoChanged(const FDialogueInfo& Info);

	void StartTypewriter(const FText& Text);
	void TypeNextChar();
	void FinishTypewriter();

	void OnAdvanceInput();
	void OnChiceClicked(int32 Index);

	void CreateChoiceButtons(const TArray<FDialogueChoice>& Choices);
	UFUNCTION()
	void HandleChoiceSelected(int32 Index);

private:
	UPROPERTY()
	TObjectPtr<UDialogueManagerSubsystem> BoundManager;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> DialogueText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> NameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> ChoiceContainer;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UChoiceButtonWidget> ChoiceButtonClass;

	TArray<FDialogueChoice> PendingChoices;
	FString FullText;
	FString CurrentText;
	int32 CharIndex = 0;

	bool bTyping = false;
	FTimerHandle TypingTimer;

	UPROPERTY(EditAnywhere)
	float TypingInterval = 0.03f;
};
