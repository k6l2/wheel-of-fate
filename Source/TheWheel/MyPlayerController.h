// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"
UCLASS()
class THEWHEEL_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
    virtual void SetupInputComponent() override;
private:
    void onConfirm();
};