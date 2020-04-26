// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

AMainCharacter::AMainCharacter()
{
	MaxHealth = 100;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

int32 AMainCharacter::GetCurrentHealth() const
{
	return CurrentHealth;
}

void AMainCharacter::SetCurrentHealth(int32 Health)
{
	CurrentHealth = Health;
}

