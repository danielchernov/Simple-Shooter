// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent *Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent *BaseMesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem *MuzzleFlash;
	UPROPERTY(EditAnywhere)
	UParticleSystem *HitEffect;
	UPROPERTY(EditAnywhere)
	USoundBase *MuzzleSound;
	UPROPERTY(EditAnywhere)
	USoundBase *HitSound;

	UPROPERTY(EditAnywhere)
	float MaxRange = 3000;
	UPROPERTY(EditAnywhere)
	float Damage = 10;

	bool GunTrace(FHitResult &HitResult, FVector &ShotDirection);
	AController *GetOwnerController() const;
};
