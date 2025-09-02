// Fill out your copyright notice in the Description page of Project Settings.


#include "DecalPoolSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"
#include "Components/DecalComponent.h"

void UDecalPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (UWorld* World = GetWorld())
	{
		//hidden actor to hold the pool
		FActorSpawnParameters Params;
		Params.Name = TEXT("DecalPoolActor");
		Params.ObjectFlags |= RF_Transient;
		AActor* Owner = World->SpawnActor<AActor>(Params);
		if (Owner)
		{
			Owner->SetActorHiddenInGame(true);
			Owner->SetActorEnableCollision(false);
			PoolOwnerActor = Owner;
		}
	}
}

void UDecalPoolSubsystem::Deinitialize()
{
	Pool.Empty();
	if (AActor* Owner = PoolOwnerActor.Get())
	{
		Owner->Destroy();
	}
	PoolOwnerActor.Reset();
	Super::Deinitialize();
}

void UDecalPoolSubsystem::EnsureCapacity(int Capacity)
{
	if (!PoolOwnerActor.IsValid())
	{
		UDecalComponent* Decal = NewObject<UDecalComponent>(PoolOwnerActor.Get());
		Decal->RegisterComponent();                 // important
		Decal->SetVisibility(false, true);
		Decal->DecalSize = FVector(64, 64, 64);
		Decal->SetVisibility(false);
		Decal->SetFadeOut(0.f, 0.f, false);        // no fade by default
		Decal->AttachToComponent(PoolOwnerActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

		FSlot Slot;
		Slot.Comp = Decal;
		Slot.bInUse = false;
		Pool.Add(Slot);
	}
}

int32 UDecalPoolSubsystem::Acquire()
{
	// Find free
	for (int32 i = 0; i < Pool.Num(); ++i)
	{
		if (!Pool[i].bInUse && Pool[i].Comp)
		{
			Pool[i].bInUse = true;
			return i;
		}
	}

	// None free → grow by a chunk
	const int32 OldSize = Pool.Num();
	EnsureCapacity(FMath::Max(8, OldSize + 32));
	for (int32 i = OldSize; i < Pool.Num(); ++i)
	{
		if (!Pool[i].bInUse)
		{
			Pool[i].bInUse = true;
			return i;
		}
	}
	return INDEX_NONE;
}

void UDecalPoolSubsystem::Release(int32 Index)
{
	if (!Pool.IsValidIndex(Index)) return;
	if (UDecalComponent* C = Pool[Index].Comp)
	{
		C->SetVisibility(false, true);
	}
	Pool[Index].bInUse = false;
}
