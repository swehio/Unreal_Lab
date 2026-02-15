// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/LabMonsterCharacter.h"
#include "Enemy/LabMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ALabMonsterCharacter::HomeLocationKey = "HomeLocation";

ALabMonsterCharacter::ALabMonsterCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ALabMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	UBlackboardComponent* BlackboardComp = Cast<AAIController>(GetController())->GetBlackboardComponent();

	HomeLocation = GetActorLocation();
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(HomeLocationKey, HomeLocation);
		UE_LOG(LogTemp, Warning, TEXT("Blackboard is not null."));
	}
}
