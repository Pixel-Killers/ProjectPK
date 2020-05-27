// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

AMainCharacter::AMainCharacter()
{
	// Active stats
	MaxHealth = 100;
	DefensePercent = 0.1f;
	BaseDamage = 20;
	AttackSpeed = 2.f;
	Currency = 0;

	// Base stats (for resetting)
	InitDefensePercent = 0.1f;
	InitBaseDamage = 20;
	InitAttackSpeed = 2.f;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

bool AMainCharacter::TakeDamage(int Damage)
{
	CurrentHealth -= Damage - Damage * FMath::Clamp(DefensePercent, 0.f, 1.f);
	return CurrentHealth <= 0;
}

int32 AMainCharacter::GetBaseDamage() const
{
	return BaseDamage;
}

int32 AMainCharacter::GetCurrency() const
{
	return Currency;
}

void AMainCharacter::SetCurrency(int32 CurrToSet)
{
	Currency = CurrToSet;
}

