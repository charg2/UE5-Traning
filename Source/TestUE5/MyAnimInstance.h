// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TESTUE5_API UMyAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UMyAnimInstance();

    virtual void NativeUpdateAnimation( float InDeltaSeconds ) override;
    void PlayAttackMontage();

private:
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=( AllowPrivateAccess = true ) )
    float Speed;

    UPROPERTY( EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=( AllowPrivateAccess = true ) )
    bool IsFalling;

    UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = ( AllowPrivateAccess = true ) )
    UAnimMontage* AttackMontage;
};
