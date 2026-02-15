#include "InteractableActor.h"

#include "Object/InteractableActor.h"
#include "Components/WidgetComponent.h"
#include "UI/InteractableIcon.h"
 
AInteractableActor::AInteractableActor()
{ 
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(RootComponent); 

	ConstructorHelpers::FClassFinder<UUserWidget> InteractableWidgetFinder(TEXT("/Game/LAB/UI/WBP_InteractableIcon"));

	if (InteractableWidgetFinder.Succeeded())
	{
		WidgetComp->SetWidgetClass(InteractableWidgetFinder.Class); 
		WidgetComp->SetDrawAtDesiredSize(true);
		WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComp->SetVisibility(false);
	} 
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	if (CachedMeshes.IsEmpty()) return;
	if (CachedMeshes.Num() == 0)
	{
		GetComponents<UMeshComponent>(CachedMeshes);
	}

	for (UMeshComponent* Comp : CachedMeshes)
	{
		Comp->SetRenderCustomDepth(true);
		Comp->SetCustomDepthStencilValue(0);
	}

	if (WidgetComp)
	{
		InteractableIconWidget = Cast<UInteractableIcon>(WidgetComp->GetUserWidgetObject());
		UpdateIconByState();
	}
}

void AInteractableActor::UpdateIconByState()
{
	if (!InteractableIconWidget||!IconTextures.Contains(InteractableState)) return;

	InteractableIconWidget->SetIconImage(IconTextures[InteractableState]); 
}
void AInteractableActor::SetInteractableState(EInteractableState NewInteractableState)
{
	InteractableState = NewInteractableState;
	UpdateIconByState();
}


FText AInteractableActor::GetInteractText_Implementation() const
{ 
	return InteractionText;
} 

void AInteractableActor::SetIconVisibility_Implementation(bool bVisible)
{
	WidgetComp->SetVisibility(bVisible);
}

void AInteractableActor::Interact_Implementation(AActor* Interactor)
{
}

void AInteractableActor::SetMeshStencil_Implementation(int StencilNum)
{
	if (CachedMeshes.Num() == 0)
	{
		return;
	}
	 
	for (UMeshComponent* Comp : CachedMeshes)
	{ 
		Comp->SetCustomDepthStencilValue(StencilNum);
	}
}

 


