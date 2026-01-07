// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ULabPlayerAnim.generated.h"

UENUM(BlueprintType)
enum class ETurnDirection : uint8
{
	Left,
	Right
};

UCLASS()
class UNREAL_LAB_API ULabPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
public:  
	
protected:
	UPROPERTY(BlueprintReadWrite)
	bool bIsTurnning = false;

	UPROPERTY(BlueprintReadWrite)
	float YawDifference;
	
public:
	void MontageStop();

protected:
	UFUNCTION(BlueprintCallable)
	bool IsDifferenceGreaterThan(float Value);
	UFUNCTION(BlueprintCallable)
	ETurnDirection CalculateTurnDirection(); 

	UFUNCTION(BlueprintCallable)
	void SetbIsTurnning(bool newbIsTurnning) { bIsTurnning = newbIsTurnning; }
	UFUNCTION(BlueprintPure)
	bool GetbIsTurnning() const { return bIsTurnning; }
	UFUNCTION(BlueprintCallable)
	void SetYawDifference(float newYawDifference) { YawDifference = newYawDifference; }
	UFUNCTION(BlueprintPure)
	float GetYawDifference() const { return YawDifference; }
};
