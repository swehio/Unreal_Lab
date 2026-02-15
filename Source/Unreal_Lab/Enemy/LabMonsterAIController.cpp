// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/LabMonsterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/Character.h"
#include "Character/LabPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


const FName ALabMonsterAIController::PatrolLocationKey = "PatrolLocation";
const FName ALabMonsterAIController::TargetPlayerKey = "TargetPlayer";
const FName ALabMonsterAIController::CanSeePlayerKey = "CanSeePlayer";
const FName ALabMonsterAIController::LastSeenLocationKey = "LastSeenLocation";
const FName ALabMonsterAIController::CanHearPlayerKey = "CanHearPlayer";
const FName ALabMonsterAIController::LastHeardLocationKey = "LaseHeardLocation";
const FName ALabMonsterAIController::StateKey = "State";

ALabMonsterAIController::ALabMonsterAIController()
{
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 2000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 70.0f;
	SightConfig->SetMaxAge(5.0f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());


	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));	
	PawnSensingComp->HearingThreshold = 1500.0f;
	PawnSensingComp->LOSHearingThreshold = 3000.0f;
	PawnSensingComp->SensingInterval = 0.25f;
	PawnSensingComp->bOnlySensePlayers = false;
	PawnSensingComp->SightRadius = 1500.0f; 
}

void ALabMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	 
	if (BehaviorTree)
	{ 
		UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
		UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent);
		RunBehaviorTree(BehaviorTree); 
	}

	if (PawnSensingComp)
	{
		PawnSensingComp->OnHearNoise.RemoveAll(this);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &ALabMonsterAIController::OnHearNoise);
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ALabMonsterAIController::InitializePatrolPoints, 0.5f, false);
}

void ALabMonsterAIController::BeginPlay()
{
	Super::BeginPlay();
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ALabMonsterAIController::OnTargetPerceptionUpdated);

	BlackboardComp = GetBlackboardComponent();
	BlackboardComp->SetValueAsEnum(StateKey, (uint8)EMonsterState::Patrol);
}


void ALabMonsterAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (!PlayerCharacter) return;

	if (Cast<ACharacter>(Actor) == PlayerCharacter)
	{
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			bool bCanSeePlayer = Stimulus.WasSuccessfullySensed(); 

			if (BlackboardComp)
			{
				BlackboardComp->SetValueAsObject(TargetPlayerKey, bCanSeePlayer ? Actor : nullptr);
				BlackboardComp->SetValueAsBool(CanSeePlayerKey, bCanSeePlayer);

				if (bCanSeePlayer)
				{
					BlackboardComp->SetValueAsVector(LastSeenLocationKey, Stimulus.StimulusLocation);
					StartChasingPlayer(Actor);
				}
				else
				{
					EMonsterState CurrentState = (EMonsterState)BlackboardComp->GetValueAsEnum(StateKey); 
					if (CurrentState == EMonsterState::Chase)
					{
						if (BlackboardComp)
						{
							BlackboardComp->SetValueAsEnum(StateKey, (uint8)EMonsterState::Investigating);
							BlackboardComp->SetValueAsVector(LastSeenLocationKey, Stimulus.StimulusLocation);
						}

						FTimerHandle StopTimer;
						GetWorldTimerManager().SetTimer(StopTimer, this, &ALabMonsterAIController::StopChasingPlayer, 5.0f, false);
					}
				}
			}
		}
	} 
}

void ALabMonsterAIController::OnHearNoise(APawn* InstigatorPawn, const FVector& Location, float Volume)
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!PlayerCharacter || !BlackboardComp) return;

	if (InstigatorPawn == PlayerCharacter)
	{
		BlackboardComp->SetValueAsBool(CanHearPlayerKey, true);
		BlackboardComp->SetValueAsVector(LastHeardLocationKey, Location);

		EMonsterState CurrentState = (EMonsterState)BlackboardComp->GetValueAsEnum(StateKey);
		if (CurrentState == EMonsterState::Patrol)
		{
			BlackboardComp->SetValueAsEnum(StateKey, (uint8)EMonsterState::Investigating);
		}

		FTimerHandle ClearHearingTimer;
		GetWorldTimerManager().SetTimer(ClearHearingTimer, [this]()
			{
				if (BlackboardComp)
				{
					BlackboardComp->SetValueAsBool(CanHearPlayerKey, false);

					EMonsterState CurrentState = (EMonsterState)BlackboardComp->GetValueAsEnum(StateKey);
					
					if (CurrentState == EMonsterState::Investigating)
					{
						BlackboardComp->SetValueAsEnum(StateKey, (uint8)EMonsterState::Patrol);
					}
				}
			}, 5.0f, false);
	}
}

void ALabMonsterAIController::StartChasingPlayer(AActor* PlayerActor)
{ 
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(StateKey, (uint8)EMonsterState::Chase);
		BlackboardComp->SetValueAsObject(TargetPlayerKey, PlayerActor);
		GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	}
}

void ALabMonsterAIController::StopChasingPlayer()
{ 
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(StateKey, (uint8)EMonsterState::Return);
		BlackboardComp->SetValueAsObject(TargetPlayerKey, nullptr);
		BlackboardComp->SetValueAsBool(CanSeePlayerKey, false);
		GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	}
}

void ALabMonsterAIController::StartSearchingLastLocation()
{ 
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(StateKey, (uint8)EMonsterState::Investigating);
	}
}

void ALabMonsterAIController::InitializePatrolPoints()
{
	PatrolPoints.Empty();
	TArray<AActor*> FoundTargetPoints;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundTargetPoints);

	for (AActor* TargetPoint : FoundTargetPoints)
	{
		PatrolPoints.Add(TargetPoint->GetActorLocation());
	}

	if (PatrolPoints.Num() > 0)
	{
		BlackboardComp->SetValueAsVector(PatrolLocationKey, PatrolPoints[0]);
		BlackboardComp->SetValueAsInt("PatrolIndex", 0);
	}

	bPatrolPointsReady = true;
}

void ALabMonsterAIController::ReportNoiseEvent(FVector NoiseLocation, float Loudness, float MaxRange)
{
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), NoiseLocation, Loudness, this, MaxRange, FName("AIHearing"));

}
