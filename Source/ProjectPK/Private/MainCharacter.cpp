// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

AMainCharacter::AMainCharacter()
{
	MaxHealth = 100;
	DefensePercent = 0.f;
	MovementSpeed = 300.f;
	Damage = 20;
	AttackSpeed = 2.f;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

