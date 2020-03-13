// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicFlyingEnemy.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include "MainCharacter.h"

ABasicFlyingEnemy::ABasicFlyingEnemy()
{
	detectRange = CreateDefaultSubobject<USphereComponent>(TEXT("Detection Range"));
	detectRange->SetupAttachment(GetRootComponent());
	detectRange->SetSphereRadius(300.f);
	detectRange->SetGenerateOverlapEvents(true);

	attackRange = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Range"));
	attackRange->SetupAttachment(GetRootComponent());
	attackRange->SetSphereRadius(150.f);
	attackRange->SetGenerateOverlapEvents(true);

	target = nullptr;
	bIsMovingRight = false;
	bIsMovingBack = false;
	bIsTimerSet = false;
	patrolEndX = 1200.f;
	interpSpeed = 4.f;
	timerDelay = 2.f;
}

void ABasicFlyingEnemy::BeginPlay()
{
	Super::BeginPlay();
	initLocation = GetActorLocation();
	locationBeforeChase = initLocation;

	detectRange->OnComponentBeginOverlap.AddDynamic(this, &ABasicFlyingEnemy::OnDetectionOverlapBegin);
	detectRange->OnComponentEndOverlap.AddDynamic(this, &ABasicFlyingEnemy::OnDetectionOverlapEnd);

	attackRange->OnComponentBeginOverlap.AddDynamic(this, &ABasicFlyingEnemy::OnAttackOverlapBegin);
	attackRange->OnComponentEndOverlap.AddDynamic(this, &ABasicFlyingEnemy::OnAttackOverlapEnd);
}

void ABasicFlyingEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MoveToTarget(DeltaSeconds);
}

void ABasicFlyingEnemy::OnDetectionOverlapBegin(UPrimitiveComponent* OverlappedComponentclass, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && !bIsMovingBack) //Uztikrinam, kad kitas aktorius validus ir ne tas pats kaip dabar.
	{
		AMainCharacter* player = Cast<AMainCharacter>(OtherActor);
		if (player) //Jei pavyko pacastinti i zaidejo klase
		{
			locationBeforeChase = GetActorLocation();
			target = player;
		}
	}
}

void ABasicFlyingEnemy::OnDetectionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(target) target = nullptr;
	GetMovementComponent()->StopActiveMovement();
}

void ABasicFlyingEnemy::MoveToTarget(float DeltaSeconds)
{
	if (target) //Tik jei turim validu taikini, judesim link jo
	{
		FVector toNormalize = target->GetActorLocation() - this->GetActorLocation();
		toNormalize.Normalize();
		AddMovementInput(toNormalize);
		if(target->GetActorLocation().X < this->GetActorLocation().X) SetActorRotation(FRotator(0.f, 180.f, 0.f));
		else SetActorRotation(FRotator(0.f)); //Nustatom, i kuria puse ziureti priesui
	}
	else //Jei tokio neturim patruliuosim
	{
		FVector currentLocation = GetActorLocation();
		if (!FMath::IsNearlyEqual(currentLocation.Z, locationBeforeChase.Z, 5.f)) //Jei nesam tam paciam auksty
		{
			bIsMovingBack = true;
			/*FVector interpVec = FMath::VInterpTo(currentLocation, locationBeforeChase, DeltaSeconds, interpSpeed);
			SetActorLocation(interpVec);*/
			FVector deltaVec = locationBeforeChase - currentLocation;
			deltaVec.Normalize();
			deltaVec /= 10;
			AddMovementInput(deltaVec);
		}
		//else bIsInterpolating = false;
		else if (bIsMovingBack && !bIsTimerSet) //Pasiruosiam pazymeti, kad nevyksta interpoliacija
		{
			bIsTimerSet = true;
			GetWorld()->GetTimerManager().SetTimer(negationTimer, this, &ABasicFlyingEnemy::NegateInterpolation, timerDelay, false);
		}

		//Vyks tik jei neinterpoliuojam i buvusia vieta
		if (bIsMovingRight && !bIsMovingBack) //I desine
		{
			AddMovementInput(FVector(1.f, 0.f, 0.f));
			FVector deltaVec = currentLocation - initLocation;
			if (deltaVec.X >= patrolEndX) bIsMovingRight = false;
			SetActorRotation(FRotator(0.f));
		}
		else if (!bIsMovingRight && !bIsMovingBack) //I kaire
		{
			AddMovementInput(FVector(-1.f, 0.f, 0.f));
			FVector deltaVec = currentLocation - initLocation;
			if (deltaVec.X <= 0) bIsMovingRight = true;
			SetActorRotation(FRotator(0.f, 180.f, 0.f));
		}
	}
}

void ABasicFlyingEnemy::NegateInterpolation()
{
	UE_LOG(LogTemp, Warning, TEXT("Pakviestas delegtatas"));
	bIsMovingBack = false;
	bIsTimerSet = false;
}

void ABasicFlyingEnemy::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponentclass, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ABasicFlyingEnemy::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

