// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UMyAnimInstance::UMyAnimInstance()
{
    static ConstructorHelpers::FObjectFinder< UAnimMontage > AM( TEXT( "AnimMontage'/Game/Animations/Greystone_Skeleton_Montage.Greystone_Skeleton_Montage'" ) );
    if ( AM.Succeeded() )
        AttackMontage = AM.Object;
}

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

void UMyAnimInstance::PlayAttackMontage()
{
    if ( Montage_IsPlaying( AttackMontage ) )
        return;

    Montage_Play( AttackMontage, 1.f );
}
