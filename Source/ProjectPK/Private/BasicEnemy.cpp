// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABasicEnemy::ABasicEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Sutvarkome kapsule
	GetCapsuleComponent()->SetCapsuleRadius(100.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(100.f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	//Tvbarkome sferas skirtas AI
	AgroRange = CreateDefaultSubobject<USphereComponent>(TEXT("Agresyvumo zona"));
	AgroRange->SetupAttachment(GetRootComponent());
	AgroRange->SetSphereRadius(100.f);

	StopRange = CreateDefaultSubobject<USphereComponent>(TEXT("Sustojimo zona"));
	StopRange->SetupAttachment(GetRootComponent());
	StopRange->SetSphereRadius(50.f);
}

// Called when the game starts or when spawned
void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

