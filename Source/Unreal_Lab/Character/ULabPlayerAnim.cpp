// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ULabPlayerAnim.h"

void ULabPlayerAnim::MontageStop()
{
	Montage_Stop(0.15f);
	SetbIsTurnning(false);
}

bool ULabPlayerAnim::IsDifferenceGreaterThan(float Value)
{ 
	return (FMath::Abs(YawDifference)>=Value);
}

ETurnDirection ULabPlayerAnim::CalculateTurnDirection()
{ 
	return FMath::Sign(YawDifference)>=0 ? ETurnDirection::Left : ETurnDirection::Right;
}
 
