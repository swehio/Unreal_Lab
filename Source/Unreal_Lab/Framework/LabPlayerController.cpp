// Fill out your copyright notice in the Description page of Project Settings.


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

    if (IA_MoveForward)
    {
        EInput->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &ALabPlayerController::MoveForward);
    }
    if (IA_MoveRight)
    {
        EInput->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &ALabPlayerController::MoveRight);
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
}

void ALabPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    CachedPlayerCharacter = Cast<ALabPlayerCharacter>(InPawn);
}

void ALabPlayerController::MoveForward(const FInputActionValue& Value)
{
    if (CachedPlayerCharacter)
        CachedPlayerCharacter->MoveForward(Value.Get<float>()); 
}

void ALabPlayerController::MoveRight(const FInputActionValue& Value)
{
    if(CachedPlayerCharacter)
		CachedPlayerCharacter->MoveRight(Value.Get<float>());
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


