#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Interactable.h"
#include "InteractableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_LAB_API UInteractableComponent : public UActorComponent, public IInteractable
{
	GENERATED_BODY()

public:	 
	UInteractableComponent();

protected: 
	virtual void BeginPlay() override;  
};
