// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AShooterCharacter::MoveForward(float AxisValue) 
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue) 
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue) 
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld() -> GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue) 
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld() -> GetDeltaSeconds());
}

void AShooterCharacter::SpawnWeapons() 
{
	Gun[0] = GetWorld() -> SpawnActor<AGun>(PrimaryGunClass);
	if(Gun[0])
	{
		Gun[0] -> AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket0"));
		Gun[0] -> SetOwner(this);
	}
	Gun[1] = GetWorld() -> SpawnActor<AGun>(SecondaryGunClass);
	if(Gun[1])
	{
		Gun[1] -> AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket1"));
		Gun[1] -> SetOwner(this);
		Gun[1] -> SetActorHiddenInGame(true);
	}
	GetMesh() -> HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	ActiveGun = 0;
}

void AShooterCharacter::SwitchWeapon() 
{
	if(Gun[ActiveGun])
		Gun[ActiveGun] -> SetActorHiddenInGame(true);
	ActiveGun = 1 - ActiveGun;
	if(Gun[ActiveGun])
		Gun[ActiveGun] -> SetActorHiddenInGame(false);

}

void AShooterCharacter::Shoot() 
{
	if(Gun[ActiveGun])
		Gun[ActiveGun] -> PullTrigger();
}

float AShooterCharacter::GetHealth() const
{
	return Health / MaxHealth;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapons();
	Health = MaxHealth;
	Dead = false;
}

bool AShooterCharacter::isDead() const
{
	return Dead;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent -> BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent -> BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent -> BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent -> BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent -> BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent -> BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);
	PlayerInputComponent -> BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent -> BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent -> BindAction(TEXT("SwitchWeapon"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SwitchWeapon);

}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) 
{
	if(Dead)
		return 0;
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageApplied = FMath::Min(Health, DamageApplied);
	Health -= DamageApplied;
	if(Health == 0)
	{
		Dead = true;
		ASimpleShooterGameModeBase* GameMode = GetWorld() -> GetAuthGameMode<ASimpleShooterGameModeBase>(); 
		if(GameMode)
			GameMode -> PawnKilled(this);
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent() -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	}
	return DamageApplied;

}

