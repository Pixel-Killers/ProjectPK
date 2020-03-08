// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "BasicFlyingEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPK_API ABasicFlyingEnemy : public APaperCharacter
{
	GENERATED_BODY()

public:
    ABasicFlyingEnemy();

protected:

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
    class USphereComponent* detectRange;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "AI | Combat")
    class AMainCharacter* target;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "AI")
    bool bIsMovingRight;

public:
    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponentclass, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void MoveToTarget();

private:
    FVector initLocation;
    float patrolEndX;
};
