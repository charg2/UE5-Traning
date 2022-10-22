// Fill out your copyright notice in the Description page of Project Settings.



#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/StaticMeshSocket.h"
#include "UObject/ConstructorHelpers.h"

#include "MyAnimInstance.h"
#include "MyCharacter.h"
#include "MyCharacterWidget.h"
#include "MyWeapon.h"
#include "MyStatComponent.h"


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
	SpringArm->SetRelativeRotation( { -35.f, 0.f, 0.f } );

	GetMesh()->SetRelativeLocationAndRotation( { 0.f, 0.0f, -88.f }, 
		{ 0.f, -90.f, 0.f } );

	static ConstructorHelpers::FObjectFinder< USkeletalMesh > SM( TEXT( "SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'" ) );
	if ( SM.Succeeded() )
		GetMesh()->SetSkeletalMesh( SM.Object );

	Stat = CreateDefaultSubobject< UMyStatComponent >( TEXT( "STAT" ) );

	HPBar = CreateDefaultSubobject< UWidgetComponent >( TEXT( "HPBAR" ) );
	HPBar->SetupAttachment( GetMesh() );
	HPBar->SetWidgetSpace( EWidgetSpace::Screen );

	static ConstructorHelpers::FClassFinder< UUserWidget > UW( TEXT( "WidgetBlueprint'/Game/UI/WBP_HPBar.WBP_HPBar_C'" ) );
	if ( UW.Succeeded() )
	{
		HPBar->SetWidgetClass( UW.Class );
		HPBar->SetDrawSize( { 200.f, 50.f } );
	}

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

	auto weapon1 = GetWorld()->SpawnActor< AMyWeapon >( FVector::ZeroVector, FRotator::ZeroRotator );
}

void AMyCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

	AnimInstance = Cast< UMyAnimInstance >( GetMesh()->GetAnimInstance() );
	if ( !AnimInstance )
		return;

	AnimInstance->OnMontageEnded.AddDynamic( this, &AMyCharacter::OnAttackMontageEnded );
	AnimInstance->OnAttackHit.AddUObject( this, &AMyCharacter::AttackCheck );

	HPBar->InitWidget();
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
 * @brief ���ݽ� �浹�� üũ�մϴ�.
 */
void AMyCharacter::AttackCheck()
{
	FHitResult hitResult;
	FCollisionQueryParams params( NAME_None, false, this );

	float attackRange = 100.f;
	float attackRadius = 50.f;

	// ä���� �̿��Ͽ� ������ �ϰڴ�.
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
	{
	    UE_LOG( LogTemp, Log, TEXT( "Hit Actor: %s" ), *hitResult.GetActor()->GetName() );

		FDamageEvent damageEvent;
		hitResult.GetActor()->TakeDamage( Stat->GetAttack(), damageEvent, GetController(), this );
	}

}

void AMyCharacter::OnAttackMontageEnded( UAnimMontage* InMontage, bool InInterrupted )
{
	IsAttacking = false;
}

float AMyCharacter::TakeDamage( 
	float               DamageAmount, 
	FDamageEvent const& DamageEvent, 
	AController*        EventInstigator,
    AActor*             DamageCauser )
{
	Stat->OnAttacked( DamageAmount );

	return DamageAmount;
}
