// Fill out your copyright notice in the Description page of Project Settings.
#include "TheWheel.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "HudMainMenu.h"
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
    if (grabbedPhysicsHandle)
    {
        const APlayerController* const playerController = Cast<APlayerController>(GetController());
        check(playerController);
        FVector mouseWorldLocation;
        FVector mouseWorldDirection;
        playerController->DeprojectMousePositionToWorld(mouseWorldLocation, mouseWorldDirection);
        grabbedPhysicsHandle->SetTargetLocation(
            mouseWorldLocation + mouseWorldDirection*wheelGrabDistance);
    }
    else if (grabbedComponent)
    {
        const FVector angularVelocity = 
			grabbedComponent->GetPhysicsAngularVelocityInDegrees();
        const float angularSpeed = angularVelocity.Size();
        static const float STOPPING_SPIN_SPEED = 1;
        if (angularSpeed <= STOPPING_SPIN_SPEED)
        {
            const FRotator rotation = grabbedComponent->GetComponentRotation();
            const float finalRoll = rotation.Roll < 0 ? 360 + rotation.Roll : rotation.Roll;
            const float finalPercent = finalRoll / 360;
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue,
                    TEXT("Spin complete! finalAngle=") + rotation.ToString() +
                    TEXT(" finalRoll=") + FString::SanitizeFloat(finalRoll) +
                    TEXT(" finalPercent=") + FString::SanitizeFloat(finalPercent));
            }
            auto pController = Cast<AMyPlayerController>(Controller);
            check(pController);
            auto hud = Cast<AHudMainMenu>(pController->GetHUD());
            for (int32 c = 0; c < hud->wheelChoices.Num(); c++)
            {
                auto& choice = hud->wheelChoices[c];
                UE_LOG(LogTemp, Warning, TEXT("choice name=%s percentStart=%f percent=%f"),
                    *choice.name, choice.percentStart, choice.percent);
                if (finalPercent >= choice.percentStart &&
                    finalPercent < choice.percentStart + choice.percent)
                {
                    if (GEngine)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Green,
                            TEXT("*** WINNER = ") + choice.name + TEXT(" ***"));
                    }
                    break;
                }
            }
            grabbedComponent = nullptr;
        }
    }
}
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAction("clicked", IE_Pressed, this, &AMyCharacter::onClicked);
    PlayerInputComponent->BindAction("clicked", IE_Released, this, &AMyCharacter::onReleased);
}
#include "HudMainMenu.h"
void AMyCharacter::onClicked()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
            TEXT("AMyCharacter::onClicked()..."));
    }
    auto controller = GetController();
    if (!controller)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
                TEXT("Controller not found!"));
        }
        return;
    }
    auto pController = Cast<AMyPlayerController>(controller);
    if (!pController)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
                TEXT("pController not found!"));
        }
        return;
    }
    auto hud = Cast<AHudMainMenu>(pController->GetHUD());
    if (!hud)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
                TEXT("HUD not found!"));
        }
        return;
    }
    if (hud->widgetInViewport())
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
                TEXT("HUD still in viewport!!!"));
        }
        return;
    }
    if (grabbedPhysicsHandle)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
                TEXT("We're already holding something!"));
        }
        return;
    }
    if (grabbedComponent)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue,
                TEXT("A spin is already in progress..."));
        }
        return;
    }
    const APlayerController* const playerController = Cast<APlayerController>(GetController());
    check(playerController);
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
    if (!hitSomething)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
                TEXT("Clicked NOTHING!"));
        }
        return;
    }
    //APawn* clickedPawn = Cast<APawn>(hitResult.GetActor());
    auto const hitActor = hitResult.GetActor();
    if (!hitActor)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
                TEXT("HitActor doesn't exist?!"));
        }
        return;
    }
    auto meshComp = hitActor->GetComponentByClass(
        UStaticMeshComponent::StaticClass());
    if (!meshComp)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
                TEXT("HitActor contains no mesh!"));
        }
        return;
    }
    auto meshCompCasted = Cast<UStaticMeshComponent>(meshComp);
    if (!meshCompCasted || !meshCompCasted->IsSimulatingPhysics())
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
                TEXT("hit actor's mesh isn't simulating physics (or doesn't exist)!"));
        }
        return;
    }
    auto comp = hitActor->GetComponentByClass(
        UPhysicsHandleComponent::StaticClass());
    if (comp)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
                TEXT("HitActor already contains a physics handle!"));
        }
        return;
    }
    wheelGrabDistance = hitResult.Distance;
    grabbedPhysicsHandle = NewObject<UPhysicsHandleComponent>(hitActor, TEXT("physicsHandle"));
    if (grabbedPhysicsHandle)
    {
        grabbedComponent = hitResult.Component.Get();
        check(grabbedComponent);
        grabbedPhysicsHandle->RegisterComponent();
        grabbedPhysicsHandle->SetTargetLocation(hitResult.Location);
        grabbedPhysicsHandle->GrabComponentAtLocation(
            grabbedComponent,
            hitResult.BoneName,
            hitResult.Location);
    }
    grabbedActor = hitActor;
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
            TEXT("Actor grabbed! name=") + grabbedActor->GetName());
    }
}
void AMyCharacter::onReleased()
{
    if (!grabbedActor)
    {
        return;
    }
    grabbedPhysicsHandle->ReleaseComponent();
    grabbedPhysicsHandle->DestroyComponent();
    grabbedComponent->WakeRigidBody();
    //grabbedActor->K2_DestroyComponent(grabbedPhysicsHandle);
    const FVector angularVelocity = 
		grabbedComponent->GetPhysicsAngularVelocityInDegrees();
    const float angularSpeed = angularVelocity.Size();
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
            TEXT("Actor released! name=") + grabbedActor->GetName() + 
            TEXT(" angularSpeed=") + FString::SanitizeFloat(angularSpeed) +
            TEXT(" angularVelocity=") + angularVelocity.ToString());
    }
    static const float MIN_SPIN_SPEED = 1000;
    grabbedActor = nullptr;
    grabbedPhysicsHandle = nullptr;
    if (angularSpeed >= MIN_SPIN_SPEED)
    {
        // preserve grabbedComponent! //
        return;
    }
    grabbedComponent = nullptr;
}
