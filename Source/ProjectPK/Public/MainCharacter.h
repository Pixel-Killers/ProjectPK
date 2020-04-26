// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "MainCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPK_API AMainCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
    AMainCharacter();
    virtual void BeginPlay() override;

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
    int32 MaxHealth;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Stats")
    int32 CurrentHealth;

public:
    int32 GetCurrentHealth() const;
    void SetCurrentHealth(int32 Health);
};
