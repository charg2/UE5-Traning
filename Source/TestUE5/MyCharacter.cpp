// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "MyAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject< USpringArmComponent >( TEXT( "SPRINGARM" ) );
	Camera = CreateDefaultSubobject< UCameraComponent >( TEXT( "CAMERA" ) );

	SpringArm->SetupAttachment( GetCapsuleComponent() );
	Camera->SetupAttachment( SpringArm );

	SpringArm->TargetArmLength = 500.f;
	SpringArm->SetRelativeRotation( FRotator{ -35.f, 0.f, 0.f } );

	GetMesh()->SetRelativeLocationAndRotation( FVector{ 0.f, 0.0f, -88.f }, 
		FRotator{ 0.f, -90.f, 0.f } );

	static ConstructorHelpers::FObjectFinder< USkeletalMesh > SM( TEXT( "SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'" ) );
	if ( SM.Succeeded() )
		GetMesh()->SetSkeletalMesh( SM.Object );
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();


	AnimInstance = Cast< UMyAnimInstance >( GetMesh()->GetAnimInstance() );
	if ( !AnimInstance )
		return;

	AnimInstance->OnMontageEnded.AddDynamic( this, &AMyCharacter::OnAttackMontageEnded );
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction( TEXT( "Jump" ), IE_Pressed, this, &AMyCharacter::Jump );
	PlayerInputComponent->BindAction( TEXT( "Attack" ), IE_Pressed, this, &AMyCharacter::Attack );


	PlayerInputComponent->BindAxis( TEXT( "UpDown" ), this, &AMyCharacter::UpDown );
	PlayerInputComponent->BindAxis( TEXT( "LeftRight" ), this, &AMyCharacter::LeftRight );
	PlayerInputComponent->BindAxis( TEXT( "Yaw" ), this, &AMyCharacter::Yaw );
}


void AMyCharacter::UpDown( float Value )
{
	if ( Value == 0.0f )
		return;

	AddMovementInput( GetActorForwardVector(), Value );
}

void AMyCharacter::LeftRight( float Value )
{
	if ( Value == 0.0f )
		return;

	AddMovementInput( GetActorRightVector(), Value );
}

void AMyCharacter::Yaw( float Value )
{
	AddControllerYawInput( Value );
}

void AMyCharacter::Attack()
{
	if ( IsAttacking )
		return;

	AnimInstance->PlayAttackMontage();

	IsAttacking = true;
}

void AMyCharacter::OnAttackMontageEnded( UAnimMontage* InMontage, bool InInterrupted )
{
	IsAttacking = false;
}
