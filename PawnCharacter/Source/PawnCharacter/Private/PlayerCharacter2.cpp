// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter2.h"

// Sets default values
APlayerCharacter2::APlayerCharacter2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

