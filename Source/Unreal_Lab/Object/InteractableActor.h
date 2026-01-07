// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "InteractableActor.generated.h"

UENUM(BlueprintType)
enum class EInteractableState : uint8
{
	None UMETA(DisplayName = "None"),
	Available UMETA(DisplayName = "Available"),           // 조건 충족
	Unavailable UMETA(DisplayName = "Unavailable"),       // 조건 불충족 
	Disabled UMETA(DisplayName = "Disabled"),             // 상호작용 불가
};

UCLASS()
class UNREAL_LAB_API AInteractableActor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	 
	AInteractableActor(); 

	virtual void BeginPlay() override;

public:

	void UpdateIconByState();

private: 
	virtual FText GetInteractText_Implementation() const;
	virtual UTexture2D* GetInteractIcon_Implementation() const;
	virtual void SetIconVisibility_Implementation(bool bVisible);
	virtual void Interact_Implementation(AActor* Interactor);
	virtual void SetMeshStencil_Implementation(int StencilNum);
	 
protected:
	//Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> SceneComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWidgetComponent> WidgetComp; 

	UPROPERTY()
	TArray< TObjectPtr<UMeshComponent>> CachedMeshes;
	 
	//Instance
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UInteractableIcon> InteractableIconWidget;
	 
	//Attrubute
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
	EInteractableState InteractableState = EInteractableState::None;

	UPROPERTY(EditAnywhere, Category = "Interactable")
	TMap<EInteractableState, UTexture2D*> IconTextures;
 
	UPROPERTY(EditAnywhere, Category = "Interactable")
	TMap<EInteractableState, FText>InteractionTexts;

	UPROPERTY(EditAnywhere, Category = "Interactable")
	UTexture2D* InteractionKeyIcon;
};
