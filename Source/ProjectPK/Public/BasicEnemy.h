// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicEnemy.generated.h"

UCLASS()
class PROJECTPK_API ABasicEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
	class USphereComponent* AgroRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
	USphereComponent* StopRange;

};
