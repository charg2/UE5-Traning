// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

#include "MyCharacter.h"
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
    auto character = Cast< AMyCharacter >( pawn );
    if ( !character )
        return;

    IsFalling = character->GetMovementComponent()->IsFalling();
    Vertical = character->UpDownValue;
    Horizontal = character->LeftRightValue;
}

void UMyAnimInstance::PlayAttackMontage()
{
    Montage_Play( AttackMontage, 1.f );
}

void UMyAnimInstance::JumpToSection( int32 InSectionIndex )
{
    FName name = GetAttackMontageName( InSectionIndex );
    Montage_JumpToSection( name, AttackMontage );
}

FName UMyAnimInstance::GetAttackMontageName( int32 InSectionIndex )
{
    return FName( *FString::Printf( TEXT( "Attack%d" ), InSectionIndex ) );
}

void UMyAnimInstance::AnimNotify_AttackHit()
{
    //UE_LOG( LogTemp, Log, TEXT( "AniNotify_AttackHit" ) );
    OnAttackHit.Broadcast();
}
