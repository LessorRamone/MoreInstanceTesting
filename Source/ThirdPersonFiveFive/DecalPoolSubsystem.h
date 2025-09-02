// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DecalPoolSubsystem.generated.h"

USTRUCT()
	struct FSlot
	{
	GENERATED_BODY()
		UPROPERTY() 
		TObjectPtr<UDecalComponent> Comp = nullptr;
		bool bInUse = false;
	};
/**
 * 
 */
UCLASS()
class UDecalPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void EnsureCapacity(int Capacity);

	int32 Acquire(); // returns pool index
	void Release(int32 Index);

	UDecalComponent* Get(int32 Index) const
	{
		return Pool.IsValidIndex(Index) ? Pool[Index].Comp : nullptr;
	}

private:
	UPROPERTY() 
	TWeakObjectPtr<AActor> PoolOwnerActor;


	UPROPERTY() TArray<FSlot> Pool;
};
