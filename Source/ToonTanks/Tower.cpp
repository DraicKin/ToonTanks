// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Tank.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (IfTankInFireRange())
    {
        RotateTurret(Tank -> GetActorLocation());
    }
}

void ATower::CheckFireCondition()
{
    if (Tank == nullptr)
    {
        return;
    }
    if (IfTankInFireRange() && Tank->bAlive)
    {
        Fire();
    }
}

bool ATower::IfTankInFireRange()
{
    if (Tank && FVector::Distance(GetActorLocation(), Tank -> GetActorLocation()) < FireRange) return true;     
    return false;
}

void ATower::HandlePawnDeath()
{
    Super::HandlePawnDeath();
    Destroy();
}