// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/DialogueDataAsset.h"
#include "DialogueWidget.generated.h"

UENUM()
enum class EDialoguePlayState : uint8
{
	Hidden,
	Typing,             // 타이핑 중
	PlayingLineEvent,   // 라인 연출(보이스/몽타주 등) 중(개념상)
	WaitingForInput,    // 다음 입력 대기
	WaitingForChoice    // 선택 대기
};

class UTextBlock;
class UVerticalBox;
class UChoiceButtonWidget;
class UDialogueManagerSubsystem;
class USoundBase;
 
UCLASS()
class UNREAL_LAB_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetManager(UDialogueManagerSubsystem* InManager) { Manager = InManager; }

	// 매니저에서 노드 변경 시 호출
	void ShowNode(const FDialogueNode& Node);

	// PlayerController에서 스킵/진행 입력으로 호출
	UFUNCTION(BlueprintCallable)
	void OnSkipOrAdvanceInput();

	void HideAll();

protected:
	// 타이핑
	void StartTyping(const FText& FullText);
	void TickTyping();
	void FinishTypingImmediately();

	// 선택지
	void BuildChoices(const TArray<FDialogueChoice>& Choices);
	UFUNCTION()
	void HandleChoiceClicked(int32 ChoiceIndex);

	// 스킵 단계별 처리
	void StopLineEventsRequest(); // Manager->StopCurrentLineEvent() 호출

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SpeakerText = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LineText = nullptr;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ChoiceList = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Typing")
	float TypingInterval = 0.05f;

	// 타이핑 중 글자 효과음(선택)
	UPROPERTY(EditDefaultsOnly, Category = "Typing")
	TObjectPtr<USoundBase> TypingSFX = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UDialogueManagerSubsystem> Manager = nullptr;

	FTimerHandle TypingTimer;

	FDialogueNode CachedNode;
	FString FullLine;
	int32 CurrentCharIndex = 0;

	EDialoguePlayState PlayState = EDialoguePlayState::Hidden;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UChoiceButtonWidget> ChoiceButtonClass;
};
