// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyCharacter.generated.h"

UCLASS()
class TESTUE5_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void PostInitializeComponents();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void UpDown( float Value );
	void LeftRight( float Value );
	void Yaw( float Value );
	void Attack();
	void AttackCheck();

	UFUNCTION()
	void OnAttackMontageEnded( UAnimMontage* InMontage, bool InInterrupted );

    virtual float TakeDamage( 
		float               DamageAmount, 
		FDamageEvent const& DamageEvent, 
		AController*        EventInstigator,
		AActor*             DamageCauser ) override;


private:
	UPROPERTY( VisibleAnywhere )
	USpringArmComponent* SpringArm;

	UPROPERTY( VisibleAnywhere )
    UCameraComponent* Camera;

	UPROPERTY( VisibleAnywhere, Category=Pawn )
    bool IsAttacking = false;

	UPROPERTY()
	class UMyAnimInstance* AnimInstance;

	UPROPERTY()
    int32 AttackIndex = 0;

public:
	UPROPERTY()
	float UpDownValue = 0;

	UPROPERTY()
    float LeftRightValue = 0;

	UPROPERTY( VisibleAnywhere )
	class UStaticMeshComponent* Weapon;

	UPROPERTY( VisibleAnywhere )
    class UMyStatComponent* Stat;

	UPROPERTY( VisibleAnywhere )
    class UWidgetComponent* HPBar;
};
