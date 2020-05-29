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

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAI")
    USphereComponent* attackRange; //Sfera, pagal kuria nustatomas atstumas is kurio reikia pulti

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CustomAI | Combat")
    class AMainCharacter* target; //Veikejas, kuri vejames

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CustomAI | Combat")
    FVector targetLocation;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CustomAI | Movemement")
    bool bIsMovingRight; //Nustato krypti judeti i kaire ar desine

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAI | Movemement")
    float patrolEndX; //Ribos X asyje, kiek judeti pasyviai pirmyn atgal

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAI | Movemement")
    float returnSpeed; //Interpoliacijos greitis

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAI | Combat")
    float attackTimerDelay; //Uz kiek laiko paleisti atakos negacijos timeri

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAI | Movemement")
    float heightDifferenceAcceptance; //Uz kiek laiko paleisti atakos negacijos timeri

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAI | Movemement")
    float patrolSpeed; //Judejimo greitis

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAI | Combat")
    int32 BaseDamage;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAI | Combat")
    bool bIsAttacking; //Parodo ar priesas dabar atakos stadijoje

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAI | Stats")
    int32 MaxHealth;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CustomAI | Stats")
    int32 CurrentHealth;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CustomAI | Loot")
    int32 CurrencyLoot;

public:
    UFUNCTION()
    void OnDetectionOverlapBegin(class UPrimitiveComponent* OverlappedComponentclass, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnDetectionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void OnAttackOverlapBegin(class UPrimitiveComponent* OverlappedComponentclass, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void MoveToTarget(float DeltaSeconds); //Apraso judejimo logika

    UFUNCTION()
    void NegateInterpolation(); //Skirtas nufalseinti interpoliacijos booli

    UFUNCTION(BlueprintCallable, Category = "CustomAI | Combat")
    bool TakeDamage(AMainCharacter* Dealer);

    UFUNCTION()
    void Attack();

private:
    FVector initLocation; //Pradine vieta, jam atsispawninus
    FVector locationBeforeChase; //Vieta pries pradedant vytis zaideja
    bool bIsMovingBack; //Ar grizta atgal
    FTimerHandle negationTimer; //I ka kaupiam interpoliacijos timerio rezultatus
    FTimerHandle attackTimer; //I ka kaupiam atakos timerio rezultatus
};
