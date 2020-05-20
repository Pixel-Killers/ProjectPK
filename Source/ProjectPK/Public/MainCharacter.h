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

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
    float DefensePercent;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
    int32 BaseDamage;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
    float AttackSpeed;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
    int32 Currency;

public:
    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool TakeDamage(int Damage);

    int32 GetBaseDamage() const;
    int32 GetCurrency() const;
    void SetCurrency(int32 CurrToSet);
};
