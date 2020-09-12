// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AShooterAIController::BeginPlay() 
{
    Super::BeginPlay();


    if(AIBehavior != nullptr)
    {
        
        RunBehaviorTree(AIBehavior);

        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        GetBlackboardComponent() -> SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn -> GetActorLocation());
    }

}

void AShooterAIController::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);
    /*APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if(PlayerPawn)
    {
        if(LineOfSightTo(PlayerPawn))
        {
            SetFocus(PlayerPawn);
            MoveToActor(PlayerPawn, AcceptanceRadius);
        }
        else
        {
            ClearFocus(EAIFocusPriority::Gameplay);
            StopMovement();
        }
    }*/

}
