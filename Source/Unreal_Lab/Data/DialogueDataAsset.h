// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogueDataAsset.generated.h"

class USoundBase;
class UAnimMontage;

USTRUCT(BlueprintType)
struct FDialogueLineEvent
{
	GENERATED_BODY();

	// 문장 보이스(대사)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> VoiceSound = nullptr;

	// 짧은 효과음(딸깍, 감탄 등)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> SFX = nullptr;

	// NPC 몽타주(손짓/고개 끄덕임)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	// 문장 시작 시 바로 재생할지(보통 true)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bPlayAtLineStart = true;
};

USTRUCT(BlueprintType)
struct FDialogueChoice
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ChoiceText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextDialogueID; 
	//이건 왜 int가 아니라 FName이지?
	/*Dialogue 노드 ID는 기획자가 직접 관리하는 데이터이기 때문에
	숫자 ID 대신 의미 기반의 FName을 사용*/

	// 선택 결과(상점 열기, 퀘스트 수락 등) - "게임 상태 변화"용
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ActionID;
};

USTRUCT(BlueprintType)
struct FDialogueNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText SpeakerName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(MultiLine=true))
	FText DialogueText;

	// Choices가 없을 때 다음으로 이동
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextDialogueID;

	// 선택지(있으면 이 노드에서 멈춤)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDialogueChoice> Choices;

	// 문장 연출(보이스/SFX/몽타주)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDialogueLineEvent LineEvent;

	// 타이핑이 끝나면 자동으로 Next로 넘어갈지(Choices 있으면 무시)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoAdvance = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float AutoAdvanceDelay = 0.2f;

	// 이 문장을 스킵 허용할지(컷신 등에서 false로)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAllowSkip = true;
};

UCLASS(BlueprintType)
class UNREAL_LAB_API UDialogueDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName StartDialogueID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, FDialogueNode> DialogueNodes;
	
};
