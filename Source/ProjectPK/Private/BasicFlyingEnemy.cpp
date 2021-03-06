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
	// Spheres
	detectRange = CreateDefaultSubobject<USphereComponent>(TEXT("Detection Range"));
	detectRange->SetupAttachment(GetRootComponent());
	detectRange->SetSphereRadius(300.f);
	detectRange->SetGenerateOverlapEvents(true);

	attackRange = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Range"));
	attackRange->SetupAttachment(GetRootComponent());
	attackRange->SetSphereRadius(150.f);
	attackRange->SetGenerateOverlapEvents(true);

	// Member variable init
	target = nullptr;
	bIsMovingRight = false;
	bIsMovingBack = false;
	bIsAttacking = false;
	patrolEndX = 1200.f;
	returnSpeed = 4.f;
	attackTimerDelay = 2.f;
	heightDifferenceAcceptance = 5.f;
	patrolSpeed = 150.f;
	BaseDamage = 20;
	CurrentHealth = 100;
	MaxHealth = 100;
	CurrencyLoot = 15;
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

	CurrentHealth = MaxHealth;
}

void ABasicFlyingEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CurrentHealth > 0)
	{
		MoveToTarget(DeltaSeconds);
		if (target)
		{
			targetLocation = target->GetActorLocation();
			//UE_LOG(LogTemp, Warning, TEXT("Taikinio vieta: %s"), *target->GetActorLocation().ToString());
		}
	}
}

void ABasicFlyingEnemy::OnDetectionOverlapBegin(UPrimitiveComponent* OverlappedComponentclass, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherComp && !bIsMovingBack) //Uztikrinam, kad kitas aktorius validus ir ne tas pats kaip dabar.
	{
		AMainCharacter* player = Cast<AMainCharacter>(OtherActor);
		UCapsuleComponent* component = Cast<UCapsuleComponent>(OtherComp);
		if (player && component) //Jei pavyko pacastinti i zaidejo klase
		{
			if(!target) locationBeforeChase = GetActorLocation(); //ifas, kad neuzfiksuotu keliu vietu random, kai jau bus pasileides
			target = player;
		}
	}
}

void ABasicFlyingEnemy::OnDetectionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		AMainCharacter* player = Cast<AMainCharacter>(OtherActor);
		UCapsuleComponent* component = Cast<UCapsuleComponent>(OtherComp);
		if (player && component && target && !bIsAttacking)
		{
			target = nullptr;
			bIsMovingBack = true; //TODO: isitikinti, kad sita vieta sitam bool'iui gera
			UE_LOG(LogTemp, Warning, TEXT("DETEKCIJOS OVERLAPO PABAIGA!!!!"));
			GetMovementComponent()->StopActiveMovement();
		}
	}
}

void ABasicFlyingEnemy::MoveToTarget(float DeltaSeconds)
{
	FVector currentLocation = GetActorLocation();
	if (currentLocation.Y != 0)
	{
		SetActorLocation(FVector(currentLocation.X, 0.f, currentLocation.Z));
	}
	if (target) //Tik jei turim validu taikini, judesim link jo
	{
		if (!bIsAttacking)
		{
			FVector toNormalize = target->GetActorLocation() - this->GetActorLocation();
			toNormalize.Normalize();
			AddMovementInput(toNormalize);
		}
		if(target->GetActorLocation().X < this->GetActorLocation().X) SetActorRotation(FRotator(0.f, 180.f, 0.f));
		else SetActorRotation(FRotator(0.f)); //Nustatom, i kuria puse ziureti priesui
	}
	else if(!bIsAttacking) //Jei tokio neturim patruliuosim
	{
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
			AddMovementInput(FVector(1.f, 0.f, 0.f) * patrolSpeed * DeltaSeconds);
			FVector deltaVec = currentLocation - initLocation;
			if (deltaVec.X >= patrolEndX) bIsMovingRight = false;
			SetActorRotation(FRotator(0.f));
		}
		else if (!bIsMovingRight && !bIsMovingBack) //I kaire
		{
			AddMovementInput(FVector(-1.f, 0.f, 0.f) * patrolSpeed * DeltaSeconds);
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
	if (OtherActor && OtherActor != this && OtherComp && !bIsMovingBack) //Uztikrinam, kad kitas aktorius validus ir ne tas pats kaip dabar.
	{
		AMainCharacter* player = Cast<AMainCharacter>(OtherActor);
		UCapsuleComponent* component = Cast<UCapsuleComponent>(OtherComp);
		if (player && component) //Jei pavyko pacastinti i zaidejo klase
		{
			//if (!target) locationBeforeChase = GetActorLocation(); //ifas, kad neuzfiksuotu keliu vietu random, kai jau bus pasileides
			target = player;
			bIsAttacking = true;
			if (!attackTimer.IsValid()) //Norim tik vieno timerio
				GetWorldTimerManager().SetTimer(attackTimer, this, &ABasicFlyingEnemy::Attack, attackTimerDelay, true);
		}
	}
}

void ABasicFlyingEnemy::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && OtherComp && !bIsMovingBack)
	{
		AMainCharacter* player = Cast<AMainCharacter>(OtherActor);
		UCapsuleComponent* component = Cast<UCapsuleComponent>(OtherComp);
		if (player && component) //Jei pavyko pacastinti i zaidejo klase
		{
			bIsAttacking = false;
			GetWorldTimerManager().ClearTimer(attackTimer); //Nebeloopinsim
		}
	}
}

void ABasicFlyingEnemy::Attack()
{
	if (target)
	{
		//target->SetCurrentHealth(target->GetCurrentHealth() - BaseDamage);
		//UE_LOG(LogTemp, Warning, TEXT("Zaidejui liko: %d HP"), target->GetCurrentHealth());
	}
}

bool ABasicFlyingEnemy::TakeDamage(AMainCharacter* Dealer)
{
	CurrentHealth -= Dealer->GetBaseDamage();
	if (CurrentHealth <= 0)
	{
		Dealer->SetCurrency(Dealer->GetCurrency() + CurrencyLoot);
	}
	return CurrentHealth <= 0;
}

