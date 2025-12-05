// Copyright Epic Games, Inc. All Rights Reserved.

#include "Unreal_LabGameMode.h"
#include "Unreal_LabCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnreal_LabGameMode::AUnreal_LabGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
