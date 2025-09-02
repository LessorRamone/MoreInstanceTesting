// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MassEntityTypes.h"
#include "DecalFragments.generated.h"


USTRUCT()
struct FDecalRequestTag : public FMassTag
{
	GENERATED_BODY()
};

USTRUCT()
struct FDecalFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Location = FVector::ZeroVector;
	UPROPERTY()
	FRotator Rotation = FRotator::ZeroRotator;

	// Unreal decals use a box; X/Y/Z are half-extents-ish via DecalSize
	UPROPERTY()
	FVector DecalSize = FVector(64.f, 64.f, 64.f);

	// this can be all the same
	// UPROPERTY()
	TObjectPtr<UMaterialInterface> Material = nullptr;

};


USTRUCT()
struct FDecalLifetimeFragment : public FMassFragment
{
	GENERATED_BODY()

	// Seconds to live; <= 0 means “don’t manage fadeout automatically”.
	UPROPERTY() float LifetimeSeconds = 0.f;

	// Internal countdown
	UPROPERTY(Transient) 
	float TimeRemaining = 0.f;

	// Index in pool, (-1 = not assigned)
	UPROPERTY(Transient) 
	int32 PoolIndex = -1;
};
