// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Interactable.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnInteractionTargetChanged, 
	UObject*,
	NewInteractable
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_LAB_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();

	virtual void BeginPlay() override;   

	void TryInteract();

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractionTargetChanged OnInteractionTargetChanged;

private:

	void UpdateInteractionTarget();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UObject> CurrentInteractable;

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> PreviouslyDetectedActors;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float TraceDistance = 500.f;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float SweepRadius = 300.f;
		
};
