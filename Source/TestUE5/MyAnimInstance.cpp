// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

void UMyAnimInstance::NativeUpdateAnimation( float InDeltaSeconds )
{
    Super::NativeUpdateAnimation( InDeltaSeconds );
    auto pawn = TryGetPawnOwner();
    if ( IsValid( pawn ) )
        Speed = pawn->GetVelocity().Size();
}
