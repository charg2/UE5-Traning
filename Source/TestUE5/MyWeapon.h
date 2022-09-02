// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

UCLASS()
class TESTUE5_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;


public:
	UFUNCTION()
    void OnCharacterOverlap( UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult );


    UPROPERTY( VisibleAnywhere )
	class UStaticMeshComponent* Weapon;

	UPROPERTY( VisibleAnywhere )
    class UBoxComponent* Trigger;
};
