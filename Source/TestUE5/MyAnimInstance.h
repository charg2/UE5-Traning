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

    virtual void NativeUpdateAnimation( float InDeltaSeconds ) override;

private:
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=( AllowPrivateAccess = true ) )
    float Speed;
};
