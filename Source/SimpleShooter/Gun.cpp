// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/DamageEvents.h"
// #include "DrawDebugHelpers.h"
// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);
	BaseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Base Mesh Component"));
	BaseMesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AGun::GunTrace(FHitResult &HitResult, FVector &ShotDirection)
{
	AController *OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
		return false;

	FVector ViewPointLocation;
	FRotator ViewPointRotation;

	OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

	FVector End = ViewPointLocation + ViewPointRotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	ShotDirection = -ViewPointRotation.Vector();

	return GetWorld()->LineTraceSingleByChannel(HitResult, ViewPointLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController *AGun::GetOwnerController() const
{
	APawn *OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;

	return OwnerPawn->GetController();
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, BaseMesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, BaseMesh, TEXT("MuzzleFlashSocket"));

	FHitResult HitResult;
	FVector ShotDirection;
	bool bSuccess = GunTrace(HitResult, ShotDirection);

	if (bSuccess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitResult.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitResult.Location, ShotDirection.Rotation());

		AActor *HitActor = HitResult.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			AController *OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}