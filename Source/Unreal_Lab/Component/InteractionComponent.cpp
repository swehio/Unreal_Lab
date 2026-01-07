#include "Component/InteractionComponent.h" 
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

UInteractionComponent::UInteractionComponent()
{ 
	PrimaryComponentTick.bCanEverTick = true; 
}

void UInteractionComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateInteractionTarget();
}

void UInteractionComponent::UpdateInteractionTarget()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	FVector Start;
	FRotator Rot;

	if (APawn* Pawn = Cast<APawn>(Owner))
	{
		if (APlayerController* PC = Cast<APlayerController>(Pawn->GetController()))
		{
			//플레이어가 실제로 보고 있는 화면 중심”을 기준으로 상호작용하기 위해 필요하다.
			PC->GetPlayerViewPoint(Start, Rot);
		}
	}

	FVector End = Start + (Rot.Vector() * TraceDistance);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceRadius);
	FHitResult HitResult;

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		Sphere
	);

	UObject* NewInteractable = nullptr;

	if (bHit && HitResult.GetActor())
	{
		UObject* HitObj = HitResult.GetActor();
		if (HitObj->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{ 
			NewInteractable = HitObj;
		}
	}
	if (NewInteractable != CurrentInteractable)
	{
		CurrentInteractable = NewInteractable;
		if(OnInteractionTargetChanged.IsBound())
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
 

