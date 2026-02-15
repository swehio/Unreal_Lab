// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/DialogueInputComponent.h"
#include "Components/AudioComponent.h"
#include "UI/DialogueWidget.h"
#include "Subsystem/DialogueManagerSubsystem.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h" 
// Sets default values for this component's properties
UDialogueInputComponent::UDialogueInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDialogueInputComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedPlayerController = Cast<APlayerController>(GetOwner());

    DialogueManager = CachedPlayerController->GetGameInstance()->GetSubsystem<UDialogueManagerSubsystem>();

    if (DialogueManager)
    {
        DialogueManager->OnDialogueStarted.AddUObject(this, &UDialogueInputComponent::HandleDialogueStarted);
        DialogueManager->OnDialogueEnded.AddUObject(this, &UDialogueInputComponent::HandleDialogueEnded);
        DialogueManager->OnDialogueNodeChanged.AddUObject(this, &UDialogueInputComponent::HandleNodeChanged);
        DialogueManager->OnDialogueActionTriggered.AddUObject(this, &UDialogueInputComponent::HandleActionTriggered);

        DialogueManager->OnDialogueLineEventStart.AddUObject(this, &UDialogueInputComponent::HandleLineEventStart);
        DialogueManager->OnDialogueLineEventStop.AddUObject(this, &UDialogueInputComponent::HandleLineEventStop);
    }

    if (DialogueWidgetClass)
    {
        DialogueWidget = CreateWidget<UDialogueWidget>(CachedPlayerController, DialogueWidgetClass);
        if (DialogueWidget)
        {
            DialogueWidget->AddToViewport();
            DialogueWidget->HideAll();
            DialogueWidget->SetManager(DialogueManager);
            UE_LOG(LogTemp, Warning, TEXT("Create Dialogue Widget"));
        }
    }
	
    if (ULocalPlayer* LocalPlayer = CachedPlayerController->GetLocalPlayer())
    {
        CachedSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    }
}


void UDialogueInputComponent::DialogueSkipOrAdvanceInput()
{
    if (DialogueWidget && DialogueManager && DialogueManager->IsRunning())
    {
        DialogueWidget->OnSkipOrAdvanceInput();
    }
}

void UDialogueInputComponent::DialogueCancle()
{
    if (DialogueWidget && DialogueManager && DialogueManager->IsRunning())
    {
        DialogueManager->EndDialogue();
    }
}

void UDialogueInputComponent::BindDialogueInput()
{
	if (!CachedPlayerController) return;
	if (UEnhancedInputComponent* EInput = Cast<UEnhancedInputComponent>(CachedPlayerController->InputComponent))
	{
        if (IA_DialogueAdvance)
        {
            EInput->BindAction(IA_DialogueAdvance, ETriggerEvent::Started, this, &UDialogueInputComponent::DialogueSkipOrAdvanceInput);
        }
        if (IA_DialogueCancle)
        {
            EInput->BindAction(IA_DialogueCancle, ETriggerEvent::Started, this, &UDialogueInputComponent::DialogueCancle);
        }
	}
}

void UDialogueInputComponent::HandleDialogueStarted()
{
    if (!DialogueWidget) return; 
        DialogueWidget->SetVisibility(ESlateVisibility::Visible); 

    if (!CachedSubsystem) return;
    CachedSubsystem->RemoveMappingContext(IMC_Gameplay);
    CachedSubsystem->AddMappingContext(IMC_Dialogue, 1);

    if (!CachedPlayerController) return;
    FInputModeGameAndUI Mode;
    CachedPlayerController->SetInputMode(Mode);
    CachedPlayerController->bShowMouseCursor = true;
}

void UDialogueInputComponent::HandleDialogueEnded()
{
    // 종료 시 보이스 정리
    if (VoiceAudioComp)
    {
        VoiceAudioComp->Stop();
        VoiceAudioComp = nullptr;
    }

    if (DialogueWidget)
    {
        DialogueWidget->HideAll();
    }

    if (!CachedPlayerController) return;
    FInputModeGameOnly Mode;
    CachedPlayerController->SetInputMode(Mode);
    CachedPlayerController->bShowMouseCursor = false;

    if (!CachedSubsystem) return;
    CachedSubsystem->RemoveMappingContext(IMC_Dialogue);
    CachedSubsystem->AddMappingContext(IMC_Gameplay, 0);

    if (APawn* P = CachedPlayerController->GetPawn())
    {
        CachedPlayerController->SetViewTargetWithBlend(P, 0.25f);
    }
}

void UDialogueInputComponent::HandleNodeChanged(const FDialogueNode& Node)
{
    if (DialogueWidget)
    {
        DialogueWidget->ShowNode(Node);
    }
}

void UDialogueInputComponent::HandleActionTriggered(FName ActionID)
{
    // 여기서는 직접 처리하지 말고(책임 분리), ShopSubsystem 등이 구독해서 처리하는 걸 추천
    UE_LOG(LogTemp, Warning, TEXT("Dialogue Action: %s"), *ActionID.ToString());
}

void UDialogueInputComponent::HandleLineEventStart(const FDialogueLineEvent& LineEvent, AActor* NPC, AActor* Interactor)
{
    // 보이스(Stop 가능하도록 AudioComponent로)
    if (LineEvent.VoiceSound)
    {
        if (VoiceAudioComp)
        {
            VoiceAudioComp->Stop();
            VoiceAudioComp = nullptr;
        }

        VoiceAudioComp = UGameplayStatics::SpawnSound2D(this, LineEvent.VoiceSound);
    }

    // 짧은 SFX는 그냥 2D로
    if (LineEvent.SFX)
    {
        UGameplayStatics::PlaySound2D(this, LineEvent.SFX);
    }
}
void UDialogueInputComponent::HandleLineEventStop(const FDialogueLineEvent& LineEvent, AActor* NPC, AActor* Interactor)
{
    // 스킵 시 보이스 중단
    if (VoiceAudioComp)
    {
        VoiceAudioComp->Stop();
        VoiceAudioComp = nullptr;
    }
}