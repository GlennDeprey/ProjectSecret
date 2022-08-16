// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseModules/SP_CharacterBase.h"

// Sets default values
ASP_CharacterBase::ASP_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASP_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASP_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASP_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

