// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
    static ConstructorHelpers::FObjectFinder< UDataTable > DATA( TEXT( "DataTable'/Game/Data/StatTable.StatTable'") );

    MyStats = DATA.Object;

}

void UMyGameInstance::Init()
{
    Super::Init();

    UE_LOG( LogTemp, Warning, TEXT( "UMyGameInstance::Init %d" ), GetStatData( 1 )->Attack );
}

FMyCahracterData* UMyGameInstance::GetStatData( int32 Level )
{
    return MyStats->FindRow< FMyCahracterData >( *FString::FromInt( Level ), TEXT( "" ) );
}
