// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicFlyingEnemy.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MainCharacter.h"

ABasicFlyingEnemy::ABasicFlyingEnemy()
{
	detectRange = CreateDefaultSubobject<USphereComponent>(TEXT("Detection Range"));
	detectRange->SetupAttachment(GetRootComponent());
	detectRange->SetSphereRadius(200.f);
	detectRange->SetGenerateOverlapEvents(true);

	target = nullptr;
	bIsMovingRight = false;
	patrolEndX = 1200.f;
}

void ABasicFlyingEnemy::BeginPlay()
{
	Super::BeginPlay();
	initLocation = GetActorLocation();
	detectRange->OnComponentBeginOverlap.AddDynamic(this, &ABasicFlyingEnemy::OnOverlapBegin);
	detectRange->OnComponentEndOverlap.AddDynamic(this, &ABasicFlyingEnemy::OnOverlapEnd);
}

void ABasicFlyingEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MoveToTarget();
}

void ABasicFlyingEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComponentclass, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this) //Uztikrinam, kad kitas aktorius validus ir ne tas pats kaip dabar.
	{
		AMainCharacter* player = Cast<AMainCharacter>(OtherActor);
		if (player) //Jeipavyko pacastinti i zaidejo klase
		{
			target = player;
		}
	}
}

void ABasicFlyingEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(target) target = nullptr;
	GetMovementComponent()->StopActiveMovement();
}

void ABasicFlyingEnemy::MoveToTarget()
{
	if (target) //Tik jei turim validu taikini, judesim link jo
	{
		FVector toNormalize = target->GetActorLocation() - this->GetActorLocation();
		toNormalize.Normalize();
		AddMovementInput(toNormalize);
	}
	else //Jei tokio neturim patruliuosim
	{
		if (bIsMovingRight) //I desine
		{
			AddMovementInput(FVector(1.f, 0.f, 0.f));
			FVector deltaVec = GetActorLocation() - initLocation;
			if (deltaVec.X >= patrolEndX) bIsMovingRight = false;
		}
		else //I kaire
		{
			AddMovementInput(FVector(-1.f, 0.f, 0.f));
			FVector deltaVec = GetActorLocation() - initLocation;
			if (deltaVec.X <= 0) bIsMovingRight = true;
		}
	}
}

