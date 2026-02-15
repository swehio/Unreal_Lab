#pragma once

#include "CoreMinimal.h"
#include "Character/LabBaseCharacter.h"
#include "LabPlayerCharacter.generated.h"
 
UCLASS()
class UNREAL_LAB_API ALabPlayerCharacter : public ALabBaseCharacter
{
	GENERATED_BODY()

public:
	ALabPlayerCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void CutMontage();

	void MoveForward(float Value);
	void MoveRight(float Value); 
	void StartSprint();
	void StopSprint(); 
	void Interact();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInteractionComponent> InteractionComp; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;  

public:
	FORCEINLINE UInteractionComponent* GetInteractionComp() const { return InteractionComp; } 

};
