// Fill out your copyright notice in the Description page of Project Settings.
#include "TheWheel.h"
#include "MyPlayerController.h"
#include "HudMainMenu.h"
void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    check(InputComponent);
    InputComponent->BindAction("confirm", IE_Pressed, this, &AMyPlayerController::onConfirm);
}
void AMyPlayerController::onConfirm()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
            TEXT("AMyPlayerController::onConfirm!"));
    }
    //auto hud = Cast<AHudMainMenu>(GetHUD());
    //if (!hud)
    //{
    //    return;
    //}
    //hud->onControllerConfirm();
}