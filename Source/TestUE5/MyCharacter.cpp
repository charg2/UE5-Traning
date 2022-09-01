// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "MyAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Engine/StaticMeshSocket.h"
#include "MyWeapon.h"

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

	//FName weaponSocket( TEXT( "hand_l_socket" ) );
	//if ( GetMesh()->DoesSocketExist( weaponSocket ) )
	//{

	//	Weapon = CreateDefaultSubobject< UStaticMeshComponent >( TEXT( "WEAPON" ) );

	//	static ConstructorHelpers::FObjectFinder< UStaticMesh > SW( TEXT( "StaticMesh'/Game/ParagonGreystone/FX/Meshes/Heroes/Greystone/SM_Greystone_Blade_01.SM_Greystone_Blade_01'" ) );
	//	if ( SW.Succeeded() )
	//		Weapon->SetStaticMesh( SW.Object );

	//	Weapon->SetupAttachment( GetMesh(), weaponSocket );
	//}

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	FName weaponSocket( TEXT( "hand_l_socket" ) );

	auto weapon = GetWorld()->SpawnActor< AMyWeapon >( FVector::ZeroVector, FRotator::ZeroRotator );
	if ( !weapon )
		return;

	weapon->AttachToComponent( GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		weaponSocket );
}

void AMyCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

	AnimInstance = Cast< UMyAnimInstance >( GetMesh()->GetAnimInstance() );
	if ( !AnimInstance )
		return;

	AnimInstance->OnMontageEnded.AddDynamic( this, &AMyCharacter::OnAttackMontageEnded );
	AnimInstance->OnAttackHit.AddUObject( this, &AMyCharacter::AttackCheck );
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
	//if ( Value == 0.0f )
	//	return;

	UpDownValue = Value;

	AddMovementInput( GetActorForwardVector(), Value );
}

void AMyCharacter::LeftRight( float Value )
{
	//if ( Value == 0.0f )
		//return;

	LeftRightValue = Value;
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
	AnimInstance->JumpToSection( AttackIndex );

	AttackIndex = ( AttackIndex + 1 ) % 3;

	IsAttacking = true;
}

/**
 * @brief 공격시 충돌을 체크합니다.
 */
void AMyCharacter::AttackCheck()
{
	FHitResult hitResult;
	FCollisionQueryParams params( NAME_None, false, this );

	float attackRange = 100.f;
	float attackRadius = 50.f;

	// 채널을 이용하여 스위핑 하겠다.
	bool result = GetWorld()->SweepSingleByChannel(
		OUT hitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * attackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere( attackRadius ),
		params );

	FVector vec    = GetActorForwardVector() * attackRange;
	FVector center = GetActorLocation() + vec * 0.5f;
	float halfHeight = attackRange * .5f + attackRadius;
	FQuat rotation = FRotationMatrix::MakeFromZ( vec ).ToQuat();
	FColor drawColor = result ? FColor::Green : FColor::Red;

	DrawDebugCapsule( GetWorld()
		,center
		,halfHeight
		,attackRadius
		,rotation
		,drawColor
		,false
		,0.2f );

	if ( result && IsValid( hitResult.GetActor() ) )
		UE_LOG( LogTemp, Log, TEXT( "Hit Actor: %s" ), *hitResult.GetActor()->GetName() );
}

void AMyCharacter::OnAttackMontageEnded( UAnimMontage* InMontage, bool InInterrupted )
{
	IsAttacking = false;
}
