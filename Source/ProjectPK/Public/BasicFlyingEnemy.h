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

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAI")
    class USphereComponent* detectRange; //Sfera, pagal kuria nustatomas zaidejo aptikimas

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CustomAI | Combat")
    class AMainCharacter* target; //Veikejas, kuri vejames

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CustomAI | Movemement")
    bool bIsMovingRight; //Nustato krypti judeti i kaire ar desine

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAI | Movemement")
    float patrolEndX; //Ribos X asyje, kiek judeti pasyviai pirmyn atgal

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAI | Movemement")
    float interpSpeed; //Interpoliacijos greitis

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAI | Movemement")
    float timerDelay; //Uz kiek laiko paleisti negacijos timeri

public:
    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponentclass, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void MoveToTarget(float DeltaSeconds);

    UFUNCTION()
    void NegateInterpolation(); //Skirtas nufalseinti interpoliacijos booli

private:
    FVector initLocation; //Pradine vieta, jam atsispawninus
    FVector locationBeforeChase; //Vieta pries pradedant vytis zaideja
    bool bIsInterpolating; //Ar interpoliuoja
    bool bIsTimerSet; //Ar timeris uzstatytas (kad amzinai nenusiresetintu)
    FTimerHandle negationTimer; //I ka kaupiam timerio rezultatus
};
