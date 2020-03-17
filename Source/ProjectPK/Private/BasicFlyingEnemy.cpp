// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicFlyingEnemy.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/PawnMovementComponent.h"
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
	bIsAttacking = false;
	patrolEndX = 1200.f;
	returnSpeed = 4.f;
	attackTimerDelay = 2.f;
	heightDifferenceAcceptance = 5.f;
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
			if(!target) locationBeforeChase = GetActorLocation(); //ifas, kad neuzfiksuotu keliu vietu random, kai jau bus pasileides
			target = player;
		}
	}
}

void ABasicFlyingEnemy::OnDetectionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (target)
	{
		target = nullptr;
		bIsMovingBack = true; //TODO: isitikinti, kad sita vieta sitam bool'iui gera
	}
	GetMovementComponent()->StopActiveMovement();
}

void ABasicFlyingEnemy::MoveToTarget(float DeltaSeconds)
{
	if (target && !bIsAttacking) //Tik jei turim validu taikini, judesim link jo
	{
		FVector toNormalize = target->GetActorLocation() - this->GetActorLocation();
		toNormalize.Normalize();
		AddMovementInput(toNormalize);
		if(target->GetActorLocation().X < this->GetActorLocation().X) SetActorRotation(FRotator(0.f, 180.f, 0.f));
		else SetActorRotation(FRotator(0.f)); //Nustatom, i kuria puse ziureti priesui
	}
	else if(!bIsAttacking) //Jei tokio neturim patruliuosim
	{
		FVector currentLocation = GetActorLocation();
		//if (!FMath::IsNearlyEqual(currentLocation.Z, locationBeforeChase.Z, heightDifferenceAcceptance)) //Jei nesam tam paciam auksty
		//{
		//	//bIsMovingBack = true; //TODO: Dar sita palikti
		//	FVector deltaVec = locationBeforeChase - currentLocation;
		//	deltaVec.Normalize(); //Pridejinesim mazas vertes
		//	deltaVec *= returnSpeed * DeltaSeconds; //Priklauso nuo greicio ir zaidimo kokybes
		//	AddMovementInput(deltaVec);
		//}
		//else if (bIsMovingBack && !negationTimer.IsValid()) //Pasiruosiam pazymeti, kad nevyksta interpoliacija
		//{ //Norim tik vieno timerio
		//	float distance = FVector::Dist(currentLocation, locationBeforeChase) / 10; //Dalinam, nes gaunam centimetrais
		//	float timerDelay = distance / returnSpeed;
		//	UE_LOG(LogTemp, Warning, TEXT("Laikas, kuri uztruksim: %f"), timerDelay);
		//	GetWorldTimerManager().SetTimer(negationTimer, this, &ABasicFlyingEnemy::NegateInterpolation, timerDelay, false);
		//}

		if (!currentLocation.Equals(locationBeforeChase, heightDifferenceAcceptance)) //Jei nesam tam paciam auksty
		{
			if (bIsMovingBack && !negationTimer.IsValid()) //Pasiruosiam pazymeti, kad nevyksta interpoliacija
			{ //Norim tik vieno timerio
				float distance = FVector::Dist(currentLocation, locationBeforeChase) / 10; //Dalinam, nes gaunam centimetrais
				float timerDelay = distance / returnSpeed;
				GetWorldTimerManager().SetTimer(negationTimer, this, &ABasicFlyingEnemy::NegateInterpolation, timerDelay, false);
			}
			//bIsMovingBack = true; //TODO: Dar sita palikti
			FVector deltaVec = locationBeforeChase - currentLocation;
			deltaVec.Normalize(); //Pridejinesim mazas vertes
			deltaVec *= returnSpeed * DeltaSeconds; //Priklauso nuo greicio ir zaidimo kokybes
			AddMovementInput(deltaVec);
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
	bIsMovingBack = false;
	GetWorldTimerManager().ClearTimer(negationTimer);
}

void ABasicFlyingEnemy::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponentclass, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsAttacking = true;
	if(!attackTimer.IsValid()) //Norim tik vieno timerio
		GetWorldTimerManager().SetTimer(attackTimer, this, &ABasicFlyingEnemy::Attack, attackTimerDelay, true);
}

void ABasicFlyingEnemy::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsAttacking = false;
	GetWorldTimerManager().ClearTimer(attackTimer); //Nebeloopinsim
}

void ABasicFlyingEnemy::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Ataka!"));
}

