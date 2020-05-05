// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI_BasicEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPK_API AAI_BasicEnemy : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
};
