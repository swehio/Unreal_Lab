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

void ALabPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {  
            if (PlayerIMC)
            {
                Subsystem->AddMappingContext(PlayerIMC, 0);
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


    
    if (UGameInstance* GI = GetGameInstance())
    {
        if (DialogueManager = GI->GetSubsystem<UDialogueManagerSubsystem>())
        {
            DialogueManager->OnDialogueInfoChanged.AddDynamic(this, &ALabPlayerController::HandleDialogueChanged);
			DialogueManager->OnDialogueEnded.AddDynamic(this, &ALabPlayerController::HandleDialogueEnded);
            if(DialogueWidgetClass)
	        {
		        DialogueWidget = CreateWidget<UDialogueWidget>(this, DialogueWidgetClass);
		        if (DialogueWidget)
		        {
			        DialogueWidget->AddToViewport();
			        DialogueWidget->SetVisibility(ESlateVisibility::Hidden);
                    DialogueWidget->Bind(DialogueManager);
			        UE_LOG(LogTemp, Warning, TEXT("Create Dialogue Widget"));
		        }
	        }
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
		    InteractionUI->SetVisibility(ESlateVisibility::Visible);
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

void ALabPlayerController::HandleDialogueChanged(const FDialogueInfo& Info)
{

	AActor* Speaker = DialogueManager->GetCurrentSpeaker();
    if (!Speaker) return;

	SetViewTargetWithBlend(Speaker, 0.5f);

	DialogueWidget->SetVisibility(ESlateVisibility::Visible);

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(DialogueWidget->TakeWidget());
	SetInputMode(InputMode);
	bShowMouseCursor = true;

	if (APawn* P = GetPawn())
	{
		P->DisableInput(this);
	}
}

void ALabPlayerController::HandleDialogueEnded()
{
	DialogueWidget->SetVisibility(ESlateVisibility::Hidden);

    FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;

	if (APawn* P = GetPawn())
	{
		P->EnableInput(this);
	}

	SetViewTargetWithBlend(CachedPlayerCharacter, 0.3f);
}
 
void ALabPlayerController::UpdateInteractionUI()
{
	if (!CurrentInteractTarget || !InteractionUI) return;

	FText Text = IInteractable::Execute_GetInteractText(CurrentInteractTarget); 

    InteractionUI->SetPromptText(Text); 
}


