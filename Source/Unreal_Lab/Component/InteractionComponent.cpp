#include "Component/InteractionComponent.h" 
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

UInteractionComponent::UInteractionComponent()
{ 
	PrimaryComponentTick.bCanEverTick = false; 
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay(); 

	FTimerHandle InteractionTraceTimer;

	GetWorld()->GetTimerManager().SetTimer(
		InteractionTraceTimer,
		this,
		&UInteractionComponent::UpdateInteractionTarget,
		0.05f,
		true
	);
} 

void UInteractionComponent::UpdateInteractionTarget()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateInteractionTargetRepeat);
	AActor* Owner = GetOwner();
	if (!Owner) return;

	// 1. LineTrace 먼저 → TargetObject 선별
	FVector Start;
	FRotator Rot;
	AActor* TargetObject = nullptr;

	if (APawn* Pawn = Cast<APawn>(Owner))
	{
		if (APlayerController* PC = Cast<APlayerController>(Pawn->GetController()))
		{
			PC->GetPlayerViewPoint(Start, Rot);
		}
	}
	FVector End = Start + (Rot.Vector() * TraceDistance);

	FHitResult LineHit;
	if (GetWorld()->LineTraceSingleByChannel(LineHit, Start, End, ECC_GameTraceChannel1))
	{
		AActor* HitActor = LineHit.GetActor();
		if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			TargetObject = HitActor;
		}
	}

#if WITH_EDITOR
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f);
#endif

	// 2. SphereTrace → 주변 감지
	TArray<FHitResult> OutHits;
	FVector SweepCenter = Owner->GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(SweepRadius);

	GetWorld()->SweepMultiByChannel(
		OutHits,
		SweepCenter,
		SweepCenter,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	);

#if WITH_EDITOR
	//DrawDebugSphere(GetWorld(), SweepCenter, SweepRadius, 12, FColor::Yellow, false, 0.1f);
#endif

	// 3. 이전 아이콘은 모두 끄기
	for (TWeakObjectPtr<AActor> PrevActor : PreviouslyDetectedActors)
	{
		if (PrevActor.IsValid() && PrevActor != TargetObject && PrevActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			IInteractable::Execute_SetIconVisibility(PrevActor.Get(), false);
		}
	}
	PreviouslyDetectedActors.Empty();

	// 4. 현재 감지된 것 중 Target 제외하고 아이콘 켜기
	for (const FHitResult& Hit : OutHits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor != TargetObject && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			IInteractable::Execute_SetIconVisibility(HitActor, true);  
			IInteractable::Execute_SetMeshStencil(HitActor, 0);
			PreviouslyDetectedActors.Add(HitActor);
		}
	}

	// 5. Target 아이콘 끄기 (선택 강조용)
	if (TargetObject && TargetObject->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		IInteractable::Execute_SetIconVisibility(TargetObject, false);  
		IInteractable::Execute_SetMeshStencil(TargetObject, 1);
	}

	// 6. 타겟 변경 시 Broadcast
	if (CurrentInteractable != TargetObject)
	{
		CurrentInteractable = TargetObject;
		OnInteractionTargetChanged.Broadcast(CurrentInteractable);  
	} 
}

void UInteractionComponent::TryInteract()
{ 
	if (CurrentInteractable)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryInteract"));
		IInteractable::Execute_Interact(
			CurrentInteractable,
			GetOwner()
		);
	}
}
 

