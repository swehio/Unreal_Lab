// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/DialogueDataAsset.h"
#include "DialogueManagerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueInfoChanged, const FDialogueInfo&, Info); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);

UCLASS()
class UNREAL_LAB_API UDialogueManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void StartDialogue(UDialogueDataAsset* InData, AActor* Speaker);
	void SelectChoice(int32 Index);
	void EndDialogue();

	const FDialogueInfo* GetCurrentInfo() const;
	AActor* GetCurrentSpeaker() const { return CurrentSpeaker.Get(); }

public:
	UPROPERTY(BlueprintAssignable)
	FOnDialogueInfoChanged OnDialogueInfoChanged;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueStarted OnDialogueStarted;
	UPROPERTY(BlueprintAssignable)
	FOnDialogueEnded OnDialogueEnded;
	 
private:
	UPROPERTY()
	TObjectPtr<UDialogueDataAsset> CurrentDialogue;

	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentSpeaker;

	FName CurrentDialogueID; 
};
