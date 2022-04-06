// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"
#include "Kismet/GameplayStatics.h"

void AToonTanksGameMode::ActorsDeath(AActor* DeadActor)
{
    if (DeadActor == Tank)
    {
        Tank -> HandlePawnDeath();
        if(ToonTanksPlayerController)
        {
            ToonTanksPlayerController -> SetPlayerEnabledState(false);
        }
	    GameOver(false);
    }
    else if (ATower* DeadTower = Cast<ATower>(DeadActor))
    {
        DeadTower -> HandlePawnDeath();
        --TowerCount;
        if (TowerCount == 0) GameOver(true);
    }
}

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();
    HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));   

    TowerCount = GetTowerCount();
    StartGame();
    if (ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnabledState(false);
    
        FTimerDelegate TimerDelegateStart = FTimerDelegate::CreateUObject
                                            (ToonTanksPlayerController, 
                                            &AToonTanksPlayerController::SetPlayerEnabledState, 
                                            true);
        FTimerHandle StartHandle;
        GetWorldTimerManager().SetTimer(StartHandle, TimerDelegateStart, StartDelay, false);
    }
}

int32 AToonTanksGameMode::GetTowerCount() const
{
    TArray <AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    return Towers.Num();
}
