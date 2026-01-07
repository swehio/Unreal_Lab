// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LabPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/InteractionComponent.h"
#include "Character/ULabPlayerAnim.h"

ALabPlayerCharacter::ALabPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	//SpringArm
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;

	//Camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	//Interaction
	InteractionComp = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComp")); 

	//Speed 설정
	NormalSpeed = 230.0f;
	SprintSpeedMultiplier = 2.2f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false; 
}

void ALabPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALabPlayerCharacter::CutMontage()
{
	ULabPlayerAnim* AI = Cast<ULabPlayerAnim>(GetMesh()->GetAnimInstance());
	if (!AI) return;

	AI->MontageStop();
}

void ALabPlayerCharacter::MoveForward(float Value)
{
	if (Value == 0.f) return;

	const FRotator YawRot(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction, Value);
	//UE_LOG(LogTemp, Display, TEXT("%f %f %f"), Direction.X, Direction.Y, Direction.Z);
}

void ALabPlayerCharacter::MoveRight(float Value)
{
	if (Value == 0.f) return;

	const FRotator YawRot(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, Value);
}

void ALabPlayerCharacter::StartSprint()
{
	if (GetCharacterMovement()) GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ALabPlayerCharacter::StopSprint()
{
	if (GetCharacterMovement()) GetCharacterMovement()->MaxWalkSpeed = NormalSpeed; 
}

void ALabPlayerCharacter::Interact()
{
	InteractionComp->TryInteract();
}
 
 
