#include "Framework/LabPlayerController.h" 
#include "Engine/GameInstance.h"
#include "Character/LabPlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h" 
#include "Blueprint/UserWidget.h"
#include "Interface/Interactable.h"
#include "UI/InteractionWidget.h"
#include "UI/DialogueWidget.h"
#include "Subsystem/DialogueManagerSubsystem.h"
#include "Component/InteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

void ALabPlayerController::BeginPlay()
{
	Super::BeginPlay();

    DialogueManager = GetGameInstance()->GetSubsystem<UDialogueManagerSubsystem>();

    if (DialogueManager)
    {
        DialogueManager->OnDialogueStarted.AddUObject(this, &ALabPlayerController::HandleDialogueStarted);
        DialogueManager->OnDialogueEnded.AddUObject(this, &ALabPlayerController::HandleDialogueEnded);
        DialogueManager->OnDialogueNodeChanged.AddUObject(this, &ALabPlayerController::HandleNodeChanged);
        DialogueManager->OnDialogueActionTriggered.AddUObject(this, &ALabPlayerController::HandleActionTriggered);

        DialogueManager->OnDialogueLineEventStart.AddUObject(this, &ALabPlayerController::HandleLineEventStart);
        DialogueManager->OnDialogueLineEventStop.AddUObject(this, &ALabPlayerController::HandleLineEventStop);
    }

    if (DialogueWidgetClass)
    {
        DialogueWidget = CreateWidget<UDialogueWidget>(this, DialogueWidgetClass);
        if (DialogueWidget)
        {
            DialogueWidget->AddToViewport();
            DialogueWidget->HideAll();
            DialogueWidget->SetManager(DialogueManager);
            UE_LOG(LogTemp, Warning, TEXT("Create Dialogue Widget"));
        }
    }

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
        if (CachedSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {  
            if (IMC_Gameplay)
            {
                CachedSubsystem->AddMappingContext(IMC_Gameplay, 0);
            }
        }
	}

    if (InteractionUIClass)
    {
        InteractionUI = CreateWidget<UInteractionWidget>(this, InteractionUIClass);
        if (InteractionUI)
        {
            InteractionUI->AddToViewport();
            InteractionUI->SetVisibility(ESlateVisibility::Hidden);
            UE_LOG(LogTemp, Warning, TEXT("Create Interaction Widget"));
        }
    }  

    if (PlayerHUDClass)
    {
        PlayerHUDWidget = CreateWidget<UUserWidget>(this, PlayerHUDClass);
        if (PlayerHUDWidget)
        {
            PlayerHUDWidget->AddToViewport(); 
            UE_LOG(LogTemp, Warning, TEXT("Create PlayerHUD Widget"));
        }
    }  
}



void ALabPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EInput = CastChecked <UEnhancedInputComponent>(InputComponent);

    if (IA_Move)
    {
        EInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ALabPlayerController::Move);
    } 
    if (IA_Look)
    {
        EInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ALabPlayerController::Look);
    }
    if (IA_Jump)
    {
        EInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &ALabPlayerController::StartJump);
        EInput->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ALabPlayerController::StopJump);
    }
    if (IA_Sprint)
    {
        EInput->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &ALabPlayerController::StartSprint);
        EInput->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &ALabPlayerController::StopSprint);
    }
    if (IA_Crouch)
    {
        EInput->BindAction(IA_Crouch, ETriggerEvent::Started, this, &ALabPlayerController::StartCrouch);
        EInput->BindAction(IA_Crouch, ETriggerEvent::Completed, this, &ALabPlayerController::StopCrouch);
    }
    if (IA_Interact)
    {
        EInput->BindAction(IA_Interact, ETriggerEvent::Started, this, &ALabPlayerController::OnInteractPressed);
    }
    if (IA_DialogueAdvance)
    {
        EInput->BindAction(IA_DialogueAdvance, ETriggerEvent::Started, this, &ALabPlayerController::DialogueSkipOrAdvanceInput);
    }
    if (IA_DialogueCancle)
    {
        EInput->BindAction(IA_DialogueCancle, ETriggerEvent::Started, this, &ALabPlayerController::DialogueCancle);
    }
}

void ALabPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    CachedPlayerCharacter = Cast<ALabPlayerCharacter>(InPawn);
	if (!CachedPlayerCharacter) return;

    UInteractionComponent* InteractionComp = CachedPlayerCharacter->GetInteractionComp();
    if (!InteractionComp) return;
	
	InteractionComp->OnInteractionTargetChanged.AddDynamic(
		this,
		&ALabPlayerController::HandleInteractionTargetChanged
	);
	 
}
 
void ALabPlayerController::UpdateInteractionUI()
{
    if (!CurrentInteractTarget || !InteractionUI) return;

    FText Text = IInteractable::Execute_GetInteractText(CurrentInteractTarget);

    InteractionUI->SetPromptText(Text);
}

void ALabPlayerController::DialogueSkipOrAdvanceInput()
{
    if (DialogueWidget && DialogueManager && DialogueManager->IsRunning())
    {
        DialogueWidget->OnSkipOrAdvanceInput();
    }
}

void ALabPlayerController::DialogueCancle()
{
    if (DialogueWidget && DialogueManager && DialogueManager->IsRunning())
    {
        DialogueManager->EndDialogue();
    }
}

void ALabPlayerController::Move(const FInputActionValue& Value)
{
    if (!CachedPlayerCharacter) return;

    
    const FVector2D MoveInput = Value.Get<FVector2D>();
    CachedPlayerCharacter->CutMontage();
     
    if(!FMath::IsNearlyZero(MoveInput.X))
        CachedPlayerCharacter->MoveForward(MoveInput.X);
    if(!FMath::IsNearlyZero(MoveInput.Y))
        CachedPlayerCharacter->MoveRight(MoveInput.Y);

} 

void ALabPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();
    AddYawInput(LookAxis.X);
	AddPitchInput(-LookAxis.Y); 
}

void ALabPlayerController::StartJump(const FInputActionValue& Value)
{
	if (CachedPlayerCharacter)
	{
		CachedPlayerCharacter->Jump(); 
	}
}

void ALabPlayerController::StopJump(const FInputActionValue& Value)
{
	if (CachedPlayerCharacter)
	{
		CachedPlayerCharacter->StopJumping();
	}
}

void ALabPlayerController::StartSprint(const FInputActionValue& Value)
{
    if (CachedPlayerCharacter)
    {
        CachedPlayerCharacter->StartSprint();
    }
}

void ALabPlayerController::StopSprint(const FInputActionValue& Value)
{
    if (CachedPlayerCharacter)
    {
        CachedPlayerCharacter->StopSprint();
    }
}

void ALabPlayerController::StartCrouch(const FInputActionValue& Value)
{
    if (CachedPlayerCharacter)
    {
        CachedPlayerCharacter->Crouch();
    }
}

void ALabPlayerController::StopCrouch(const FInputActionValue& Value)
{
    if (CachedPlayerCharacter)
    {
        CachedPlayerCharacter->UnCrouch();
    }
}

void ALabPlayerController::OnInteractPressed(const FInputActionValue& Value)
{
	if (CachedPlayerCharacter)
	{
        CachedPlayerCharacter->Interact();
	}
}

void ALabPlayerController::HandleInteractionTargetChanged(UObject* NewTarget)
{
    CurrentInteractTarget = NewTarget;
    if (InteractionUI)
    {
	    if (CurrentInteractTarget)
	    {
		    InteractionUI->SetVisibility(ESlateVisibility::HitTestInvisible);
            UpdateInteractionUI();
            UE_LOG(LogTemp, Warning, TEXT("CurrentInteractTarget Is Not Null"));
	    }
	    else 
	    {
		    InteractionUI->SetVisibility(ESlateVisibility::Hidden); 
            UE_LOG(LogTemp, Warning, TEXT("CurrentInteractTarget Is Null"));
	    } 
    }
} 

void ALabPlayerController::HandleDialogueStarted()
{
    if (DialogueWidget)
    {
        DialogueWidget->SetVisibility(ESlateVisibility::Visible);
    }

    CachedSubsystem->RemoveMappingContext(IMC_Gameplay);
    CachedSubsystem->AddMappingContext(IMC_Dialogue, 1); 

    FInputModeGameAndUI Mode;   
    SetInputMode(Mode);
    bShowMouseCursor = true;
}

void ALabPlayerController::HandleDialogueEnded()
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

    FInputModeGameOnly Mode;
    SetInputMode(Mode);
    bShowMouseCursor = false;

    CachedSubsystem->RemoveMappingContext(IMC_Dialogue);
    CachedSubsystem->AddMappingContext(IMC_Gameplay, 0);

    if (APawn* P = GetPawn())
    {
        SetViewTargetWithBlend(P, 0.25f);
    }
}

void ALabPlayerController::HandleNodeChanged(const FDialogueNode& Node)
{
    if (DialogueWidget)
    {
        DialogueWidget->ShowNode(Node);
    }
}

void ALabPlayerController::HandleActionTriggered(FName ActionID)
{
    // 여기서는 직접 처리하지 말고(책임 분리), ShopSubsystem 등이 구독해서 처리하는 걸 추천
    UE_LOG(LogTemp, Warning, TEXT("Dialogue Action: %s"), *ActionID.ToString());
}

void ALabPlayerController::HandleLineEventStart(const FDialogueLineEvent& LineEvent, AActor* NPC, AActor* Interactor)
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
void ALabPlayerController::HandleLineEventStop(const FDialogueLineEvent& LineEvent, AActor* NPC, AActor* Interactor)
{
    // 스킵 시 보이스 중단
    if (VoiceAudioComp)
    {
        VoiceAudioComp->Stop();
        VoiceAudioComp = nullptr;
    }
}






