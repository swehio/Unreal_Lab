#include "Component/InteractableComponent.h"
#include "Components/WidgetComponent.h"
 
UInteractableComponent::UInteractableComponent()
{ 
	PrimaryComponentTick.bCanEverTick = false;
	 
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
}

void UInteractableComponent::Interact(AActor* Interactor)
{
}

FText UInteractableComponent::GetInteractText() const
{
	return FText();
}

UTexture2D* UInteractableComponent::GetInteractIcon() const
{
	return nullptr;
}

 

 

