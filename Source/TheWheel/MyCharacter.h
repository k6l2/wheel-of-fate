// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"
UCLASS()
class THEWHEEL_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	AMyCharacter();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
    void onClicked();
    void onReleased();
private:
    float wheelGrabDistance;
    UPhysicsHandleComponent* grabbedPhysicsHandle;
    UPrimitiveComponent* grabbedComponent;
    AActor* grabbedActor;
};