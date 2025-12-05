// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/LabBaseCharacter.h"
#include "LabPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_LAB_API ALabPlayerCharacter : public ALabBaseCharacter
{
	GENERATED_BODY()

public:
	ALabPlayerCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value); 
	void StartSprint();
	void StopSprint();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

	
};
