#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(BlueprintType)
class UInteractable : public UInterface
{
    GENERATED_BODY()
};

class IInteractable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Interact(AActor* Interactor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    FText GetInteractText() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    UTexture2D* GetInteractIcon() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void SetIconVisibility(bool bVisible);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void SetMeshStencil(int StencilNum);
};
