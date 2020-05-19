// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

AMainCharacter::AMainCharacter()
{
	MaxHealth = 100;
	DefensePercent = 0.1f;
	BaseDamage = 20;
	AttackSpeed = 2.f;
	Currency = 0;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void AMainCharacter::TakeDamage(int Damage)
{
	CurrentHealth -= Damage - Damage * DefensePercent;
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

