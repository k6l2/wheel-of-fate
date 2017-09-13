// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "GameFramework/HUD.h"
#include "Runtime/UMG/Public/UMG.h"
#include "HudMainMenu.generated.h"
/**
 * 
 */
USTRUCT()
struct FWheelChoice
{
    GENERATED_BODY()
    UPROPERTY()
    float percent;
    // The beginning of the pie slice
    UPROPERTY()
    float percentStart;
    UPROPERTY()
    FColor color;
    UPROPERTY()
    FString name;
};
UCLASS()
class THEWHEEL_API AHudMainMenu : public AHUD
{
	GENERATED_BODY()
public:
    AHudMainMenu();
    virtual void PostInitializeComponents() override;
    bool widgetInViewport() const;
    //void onControllerConfirm();
public:
    UPROPERTY()
    TArray<FWheelChoice> wheelChoices;
private:
    UPROPERTY(EditDefaultsOnly, Category = UI)
    TSubclassOf<UUserWidget> widgetTemplate;
    UPROPERTY()
    UUserWidget* widgetInstance;
};