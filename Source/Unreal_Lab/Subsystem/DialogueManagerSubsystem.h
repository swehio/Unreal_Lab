// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/DialogueDataAsset.h"
#include "DialogueManagerSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDialogueStarted);
DECLARE_MULTICAST_DELEGATE(FOnDialogueEnded);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDialogueNodeChanged, const FDialogueNode&);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDialogueActionTriggered, FName /*ActionID*/);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnDialogueLineEventStart, const FDialogueLineEvent& /*LineEvent*/, AActor* /*NPC*/, AActor* /*Interactor*/);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnDialogueLineEventStop, const FDialogueLineEvent& /*LineEvent*/, AActor* /*NPC*/, AActor* /*Interactor*/);


UCLASS()
class UNREAL_LAB_API UDialogueManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void StartDialogue(UDialogueDataAsset* InData, AActor* InInteractor, AActor* InNPC);
	void EndDialogue();

	void Advance(); 
	void SelectChoice(int32 Index);

	void StopCurrentLineEvent();

	bool IsRunning() const { return bRunning; } 
	const FDialogueNode* GetCurrentNode() const { return CurrentNodePtr; };
	TObjectPtr<AActor> GetNPC() const { return NPC.Get(); }
	TObjectPtr<AActor> GetInteractor() const { return Interactor.Get(); }

	FOnDialogueStarted OnDialogueStarted;
	FOnDialogueEnded OnDialogueEnded;
	FOnDialogueNodeChanged OnDialogueNodeChanged;
	FOnDialogueActionTriggered OnDialogueActionTriggered;

	FOnDialogueLineEventStart OnDialogueLineEventStart;
	FOnDialogueLineEventStop OnDialogueLineEventStop;

private:
	void MoveToNode(FName DialogueID);
	void ExecuteActionIfAny(FName ActionID);
	 
private:
	UPROPERTY()
	TObjectPtr<UDialogueDataAsset> Data = nullptr; 

	UPROPERTY()
	TWeakObjectPtr<AActor> Interactor;

	UPROPERTY()
	TWeakObjectPtr<AActor> NPC;

	bool bRunning = false;

	FName CurrentID = NAME_None;
	const FDialogueNode* CurrentNodePtr = nullptr;

	// 현재 라인 이벤트 캐시(스킵 중단용)
	FDialogueLineEvent CurrentLineEvent;
};
