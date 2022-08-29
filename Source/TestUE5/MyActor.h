// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class TESTUE5_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick( float DeltaTime ) override;

private:

	UPROPERTY( VisibleAnywhere )
    UStaticMeshComponent* Mesh;

	UPROPERTY( EditAnywhere, Category = Stat )
	int32 Hp;

	UPROPERTY( VisibleAnywhere, Category = Stat )
	int32 Mp;

	UPROPERTY( VisibleAnywhere, Category = Stat )
	float RotateSpeed = 30.0f;
};
