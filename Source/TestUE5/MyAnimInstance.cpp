// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

void UMyAnimInstance::NativeUpdateAnimation( float InDeltaSeconds )
{
    Super::NativeUpdateAnimation( InDeltaSeconds );
    auto pawn = TryGetPawnOwner();
    if ( !IsValid( pawn ) )
        return;

    Speed = pawn->GetVelocity().Size();
    auto character = Cast< ACharacter >( pawn );
    if ( !character )
        return;

    IsFalling = character->GetMovementComponent()->IsFalling();
}
