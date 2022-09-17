// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStatComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMyStatComponent::UMyStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	bWantsInitializeComponent = true;

	Level = 1;
}


// Called when the game starts
void UMyStatComponent::BeginPlay()
{
	Super::BeginPlay();


}

void UMyStatComponent::InitializeComponent()
{
    Super::InitializeComponent();

	Level = 1;
	SetLevel( Level );
}

void UMyStatComponent::SetLevel( int32 InLevel )
{
	auto myGameInstance = Cast< UMyGameInstance >( UGameplayStatics::GetGameInstance( GetWorld() ) );
	if ( !myGameInstance )
		return;

	auto statData = myGameInstance->GetStatData( InLevel );
	if ( !statData )
		return;

	Level = statData->Level;
	Hp = statData->MaxHp;
	Attack = statData->Attack;
}

void UMyStatComponent::OnAttacked( float InDamageAmount )
{
	Hp -= InDamageAmount;
	if ( Hp < 0 )
		Hp = 0;

	UE_LOG( LogTemp, Warning, TEXT( "OnAttacked InDamage%f Hp%d" ),InDamageAmount, Hp );
}

