// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractableIcon.generated.h"
 
UCLASS()
class UNREAL_LAB_API UInteractableIcon : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "InteractableIcon")
	void SetIconImage(UTexture2D* Texture);

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;
};
