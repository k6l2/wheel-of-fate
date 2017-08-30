// Fill out your copyright notice in the Description page of Project Settings.
#include "TheWheel.h"
#include "HudMainMenu.h"
AHudMainMenu::AHudMainMenu()
{
}
void AHudMainMenu::PostInitializeComponents()
{
    auto world = GetWorld();
    if (!widgetTemplate || !world)
    {
        return;
    }
    widgetInstance = CreateWidget<UUserWidget>(world, widgetTemplate);
    if (!widgetInstance)
    {
        return;
    }
    widgetInstance->AddToViewport();
    AActor::PostInitializeComponents();
}