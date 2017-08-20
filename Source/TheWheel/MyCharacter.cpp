// Fill out your copyright notice in the Description page of Project Settings.
#include "TheWheel.h"
#include "MyCharacter.h"
AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}
void AMyCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAction("clicked", IE_Pressed, this, &AMyCharacter::onClicked);
    PlayerInputComponent->BindAction("clicked", IE_Released, this, &AMyCharacter::onReleased);
}
void AMyCharacter::onClicked()
{
    const APlayerController* const playerController = Cast<APlayerController>(GetController());
    // Get the coordinates of the mouse from our controller  //
    float mousePositionX;
    float mousePositionY;
    playerController->GetMousePosition(mousePositionX, mousePositionY);
    // Do a trace and see if there the position intersects something in the world  //
    const FVector2D mousePosition(mousePositionX, mousePositionY);
    FHitResult hitResult;
    const bool bTraceComplex = false;
    const bool hitSomething = playerController->GetHitResultAtScreenPosition(
        mousePosition, ECC_Visibility, bTraceComplex, hitResult);
    if (hitSomething)
    {
        //APawn* clickedPawn = Cast<APawn>(hitResult.GetActor());
        if (hitResult.GetActor())
        {
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
                    TEXT("Actor clicked! name=") + hitResult.GetActor()->GetName());
            }
        }
        ///TODO: create a physics handle if we're clicking on a physics-enabled actor
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
                TEXT("Clicked NOTHING!"));
        }
    }
}
void AMyCharacter::onReleased()
{
}