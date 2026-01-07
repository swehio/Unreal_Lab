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

	virtual void Interact(AActor* Interactor);

	virtual FText GetInteractText() const;

	virtual UTexture2D* GetInteractIcon() const;
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UWidgetComponent> WidgetComp; 

};
