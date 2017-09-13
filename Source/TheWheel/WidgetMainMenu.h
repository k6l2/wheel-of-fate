// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Blueprint/UserWidget.h"
#include "WidgetMainMenu.generated.h"
/**
 * 
 */
UCLASS()
class THEWHEEL_API UWidgetMainMenu : public UUserWidget
{
	GENERATED_BODY()
public:
    //void onHudConfirm();
protected:
    UFUNCTION(BlueprintCallable, Category = Callbacks)
    void onAddOptionClicked();
    UFUNCTION(BlueprintCallable, Category = Callbacks)
    void onSpinTheWheelClicked();
    UFUNCTION(BlueprintCallable, Category = Callbacks)
    FEventReply handleOnKeyDown(const FKeyEvent& InKeyEvent);
    UFUNCTION(BlueprintCallable, Category = Callbacks)
    void editTextOptionName_OnTextCommitted(
        const FText & Text,
        ETextCommit::Type CommitMethod);
private:
    UPROPERTY(EditDefaultsOnly, Category = UI)
    TSubclassOf<UUserWidget> widgetTemplateOptions;
    UPROPERTY()
    UMaterialInstanceDynamic* wheelDynamicFrontMaterial;
    UPROPERTY()
    UTexture2D* wheelChoicesTexture;
};