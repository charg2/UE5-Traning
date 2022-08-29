// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject< UStaticMeshComponent >( TEXT( "MESH" ) );
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder< UStaticMesh > SM( TEXT( "StaticMesh'/Game/StarterContent/Props/SM_Couch.SM_Couch'" ) );

	if ( SM.Succeeded() )
		Mesh->SetStaticMesh( SM.Object );
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG( LogTemp, Warning, TEXT( "In begin Play: %d" ), 100 );
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG( LogTemp, Error, TEXT( "In Tick: %f" ), DeltaTime );

	AddActorLocalRotation( FRotator( 0.f, RotateSpeed * DeltaTime, 0.f ) );
}

