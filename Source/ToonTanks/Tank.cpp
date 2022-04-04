// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"

ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm -> SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera -> SetupAttachment(SpringArm);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent -> BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent -> BindAxis(TEXT("Turn"), this, &ATank::Turn);
    PlayerInputComponent -> BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	PlayercontrollerRef = Cast<APlayerController>(GetController());
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);    

    if (PlayercontrollerRef)
    {        
        FHitResult Hit;
        PlayercontrollerRef -> GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,
            false, 
            Hit);  
        RotateTurret(Hit.ImpactPoint);   
    }
}

void ATank::Move(float Value)
{
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    FVector DeltaLocation(0.f);
    DeltaLocation.X = Value * DeltaTime * Speed;
    AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    FRotator DeltaRotation(0.f);
    DeltaRotation.Yaw = Value * DeltaTime * TurnRate;
    AddActorLocalRotation(DeltaRotation, true);
}

void ATank::HandlePawnDeath()
{
    Super::HandlePawnDeath();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}