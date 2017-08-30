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
protected:
    UFUNCTION(BlueprintCallable, Category = Callbacks)
    void onAddOptionClicked();
private:
    UPROPERTY(EditDefaultsOnly, Category = UI)
    TSubclassOf<UUserWidget> widgetTemplateOptions;
};