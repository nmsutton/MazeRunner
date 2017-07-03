// Fill out your copyright notice in the Description page of Project Settings.


#include "mouse_body.h"


// Sets default values
Amouse_body::Amouse_body()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Amouse_body::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Amouse_body::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Amouse_body::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

