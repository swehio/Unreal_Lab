// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "LabMonsterAIController.generated.h"

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Patrol = 0,
	Investigating = 1,
	Suspicion = 2,
	Chase = 3,
	Attack = 4,
	Return = 5,
	Death = 6 
}; 

UCLASS()
class UNREAL_LAB_API ALabMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	ALabMonsterAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnHearNoise(APawn* InstigatorPawn, const FVector& Location, float Volume);

	UFUNCTION()
	void StartChasingPlayer(AActor* PlayerActor);

	UFUNCTION()
	void StopChasingPlayer();

	UFUNCTION()
	void StartSearchingLastLocation(); 

private:
	void InitializePatrolPoints();

	void ReportNoiseEvent(FVector NoiseLocation, float Loudness = 1.0f, float MaxRange = 2000.0f);

public:
	bool bPatrolPointsReady = false;
	TArray<FVector> PatrolPoints;

protected:    
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UBlackboardComponent> BlackboardComp = nullptr;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<class UBehaviorTree> BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr <UAIPerceptionComponent> PerceptionComp;

	UPROPERTY(VisibleAnywhere, Category = "AI|Perception");
	TObjectPtr <class UAISenseConfig_Sight> SightConfig;

	UPROPERTY(VisibleAnywhere, Category = "AI|Perception");
	TObjectPtr <class UAISenseConfig_Hearing> HearingConfig;

	UPROPERTY(VisibleAnywhere, Category = "AI|Sensing")
	TObjectPtr <class UPawnSensingComponent> PawnSensingComp;
 
	static const FName PatrolLocationKey;  
	static const FName TargetPlayerKey;
	static const FName CanSeePlayerKey;
	static const FName LastSeenLocationKey;
	static const FName LastHeardLocationKey;
	static const FName CanHearPlayerKey;
	static const FName StateKey;
};
