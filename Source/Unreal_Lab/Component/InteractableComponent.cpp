#include "Component/InteractableComponent.h"
 
UInteractableComponent::UInteractableComponent()
{ 
	PrimaryComponentTick.bCanEverTick = false;
	 
}


// Called when the game starts
void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

 

