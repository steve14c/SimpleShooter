// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"


void ASimpleShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner) 
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);
    UUserWidget* EndScreen;
    if(bIsWinner)
        EndScreen = CreateWidget(this, WinScreenClass);
    else
    {
        EndScreen = CreateWidget(this, LoseScreenClass);
    }
    if(EndScreen)
        EndScreen -> AddToViewport();
    HUDScreen -> RemoveFromViewport();
    GetWorldTimerManager().SetTimer(RestartTimer, this, &ASimpleShooterPlayerController::RestartLevel, RestartDelay);

}

void ASimpleShooterPlayerController::BeginPlay() 
{
    Super::BeginPlay();
    HUDScreen = CreateWidget(this, HUDScreenClass);
    HUDScreen -> AddToViewport();
}
