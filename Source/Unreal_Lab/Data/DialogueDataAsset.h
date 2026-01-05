// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogueDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FDialogueChoice
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText ChoiceText;

	UPROPERTY(EditAnywhere)
	FName NextDialogueID; 
	//이건 왜 int가 아니라 FName이지?
	/*Dialogue 노드 ID는 기획자가 직접 관리하는 데이터이기 때문에
	숫자 ID 대신 의미 기반의 FName을 사용*/
};

USTRUCT(BlueprintType)
struct FDialogueInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText SpearkerName;

	UPROPERTY(EditAnywhere, meta=(MultiLine=true))
	FText DialogueText;

	UPROPERTY(EditAnywhere)
	TArray<FDialogueChoice> Choices;
};

UCLASS(BlueprintType)
class UNREAL_LAB_API UDialogueDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName StartDialogueID;

	UPROPERTY(EditAnywhere)
	TMap<FName, FDialogueInfo> DialogueMap;
	
};
