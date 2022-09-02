// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"

#include "MyCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AMyWeapon::AMyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject< UStaticMeshComponent >( TEXT( "WEAPON" ) );
	Trigger = CreateDefaultSubobject< UBoxComponent >( TEXT( "TRIGGER" ) );

	static ConstructorHelpers::FObjectFinder< UStaticMesh > SW( TEXT( "StaticMesh'/Game/ParagonGreystone/FX/Meshes/Heroes/Greystone/SM_Greystone_Blade_01.SM_Greystone_Blade_01'" ) );
	if ( SW.Succeeded() )
		Weapon->SetStaticMesh( SW.Object );

	Weapon->SetupAttachment( RootComponent );
	Trigger->SetupAttachment( Weapon );

	Weapon->SetCollisionProfileName( TEXT( "MyCollectible" ) );
	Trigger->SetCollisionProfileName( TEXT( "MyCollectible" ) );
	Trigger->SetBoxExtent( { 30.0f, 30.0f, 30.0f } );
}

// Called when the game starts or when spawned
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
}


void AMyWeapon::OnCharacterOverlap( 
	UPrimitiveComponent* PrimitiveComponent, 
	AActor*              InOtherActor,
    UPrimitiveComponent* PrimitiveComponent1, 
	int                  Distance, 
	bool                 FromSweep, 
	const FHitResult&    SweepResult )
{
	UE_LOG( LogTemp, Log, TEXT( "Overalpped" ) );

	auto myCharacter = Cast< AMyCharacter >( InOtherActor );
	if ( !myCharacter )
		return;

	FName weaponSocket( TEXT( "hand_l_socket" ) );

	AttachToComponent( myCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		weaponSocket );

}

void AMyWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic( this, &AMyWeapon::OnCharacterOverlap );
}
