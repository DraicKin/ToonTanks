// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Tank.h"
#include "Tower.h"
#include "Kismet/GameplayStatics.h"

void AToonTanksGameMode::ActorsDeath(AActor* DeadActor)
{
    if (DeadActor == Tank)
    {
        Tank -> HandlePawnDeath();
        if(Tank -> GetPlayerControllerRef())
        {
            Tank -> DisableInput(Tank -> GetPlayerControllerRef());
            Tank -> GetPlayerControllerRef() -> bShowMouseCursor = false;
        }     
    }
    else if (ATower* DeadTower = Cast<ATower>(DeadActor))
    {
        DeadTower -> HandlePawnDeath();
    }
}

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
}