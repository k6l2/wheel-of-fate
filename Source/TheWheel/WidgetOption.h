// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Blueprint/UserWidget.h"
#include "WidgetOption.generated.h"
/**
 * 
 */
UCLASS()
class THEWHEEL_API UWidgetOption : public UUserWidget
{
	GENERATED_BODY()
public:
    UWidgetOption(const FObjectInitializer & ObjectInitializer);
    float percentage() const;
    void percentage(float newPercent);
    void setOptionText(const FText& inText);
protected:
    UFUNCTION(BlueprintCallable, Category = Callbacks)
    void onRemoveClicked();
    UFUNCTION(BlueprintCallable, Category = Callbacks)
    void onProbabilityChanged(float newValue);
private:
    // soooo... this isn't actually useful?..
    float oldPercentage;
};