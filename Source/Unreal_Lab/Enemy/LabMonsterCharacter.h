// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/LabBaseCharacter.h"
#include "LabMonsterCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_LAB_API ALabMonsterCharacter : public ALabBaseCharacter
{
	GENERATED_BODY()
	
public:
	ALabMonsterCharacter();

protected:
	virtual void BeginPlay() override; 

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsStaticMonster; 

	FVector HomeLocation;

	static const FName HomeLocationKey;

};
