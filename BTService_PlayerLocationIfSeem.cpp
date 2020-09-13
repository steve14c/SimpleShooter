// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeem.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTService_PlayerLocationIfSeem::UBTService_PlayerLocationIfSeem() 
{
    NodeName = TEXT("UpdatePlayerLocationIfSeem");    
}


void UBTService_PlayerLocationIfSeem::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if(!PlayerPawn)
        return;
    if(!OwnerComp.GetBlackboardComponent())
        return;
    AAIController* AIController = OwnerComp.GetAIOwner();
    if(!AIController)
        return;
    if(AIController -> LineOfSightTo(PlayerPawn))
    {
        OwnerComp.GetBlackboardComponent() -> SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn -> GetActorLocation());
    }
    else
    {
        OwnerComp.GetBlackboardComponent() -> ClearValue(GetSelectedBlackboardKey());
    }
}

