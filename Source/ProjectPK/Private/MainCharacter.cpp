// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

AMainCharacter::AMainCharacter()
{
	MaxHealth = 100;
	DefensePercent = 0.1f;
	BaseDamage = 20;
	AttackSpeed = 2.f;
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

