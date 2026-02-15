// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask/BTTask_FindRandomPatrolLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BTTask_FindRandomPatrolLocation.h"

EBTNodeResult::Type UBTTask_FindRandomPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	const FVector Home = BlackboardComp->GetValueAsVector("HomeLocation");

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(AIController->GetWorld());
	if (!NavSys) return EBTNodeResult::Failed;

	FNavLocation RandomLocation;
	if (NavSys->GetRandomReachablePointInRadius(Home, PatrolRadius, RandomLocation))
	{
		BlackboardComp->SetValueAsVector("PatrolLocation", RandomLocation.Location);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
