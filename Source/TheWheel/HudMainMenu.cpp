// Fill out your copyright notice in the Description page of Project Settings.
#include "TheWheel.h"
#include "HudMainMenu.h"
#include "WidgetMainMenu.h"
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
    //FInputModeUIOnly inputMode;
    //world->GetFirstPlayerController()->SetInputMode(inputMode);
    widgetInstance->SetKeyboardFocus();
    AActor::PostInitializeComponents();
}
bool AHudMainMenu::widgetInViewport() const
{
    return widgetInstance && widgetInstance->IsInViewport();
}
//void AHudMainMenu::onControllerConfirm()
//{
//    auto widgetMainMenu = Cast<UWidgetMainMenu>(widgetInstance);
//    if (!widgetMainMenu)
//    {
//        return;
//    }
//    widgetMainMenu->onHudConfirm();
//}