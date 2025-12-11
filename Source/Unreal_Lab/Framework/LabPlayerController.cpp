#include "Framework/LabPlayerController.h"
#include "Character/LabPlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

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
}

void ALabPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    CachedPlayerCharacter = Cast<ALabPlayerCharacter>(InPawn);
}

void ALabPlayerController::Move(const FInputActionValue& Value)
{
    if (!CachedPlayerCharacter) return;

    
    const FVector2D MoveInput = Value.Get<FVector2D>();
     
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


