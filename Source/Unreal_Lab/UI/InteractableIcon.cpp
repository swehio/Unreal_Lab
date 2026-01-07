// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractableIcon.h"
#include "Components/Image.h"

void UInteractableIcon::SetIconImage(UTexture2D* Texture)
{
	if (Icon && Texture)
	{
		Icon->SetBrushFromTexture(Texture);
	}
}
