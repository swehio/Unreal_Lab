// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTask_MonsterAttack.h"
#include "Enemy/LabMonsterCharacter.h"
#include "Enemy/LabMonsterAIController.h"

EBTNodeResult::Type UBTTask_MonsterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();

    if (!AICon) return EBTNodeResult::Failed;

	ALabMonsterCharacter* Monster = Cast<ALabMonsterCharacter>(AICon->GetPawn());

    if (Monster)
    {
		//Monster->Attack();
		return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
