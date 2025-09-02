// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicActorMgrBase.h"
#include "DecalPoolSubsystem.h"
#include "MassEntitySubsystem.h"
#include "MassCommonFragments.h"
#include "DecalFragments.h"
#include "Components/DecalComponent.h"


// Sets default values for this component's properties
UDynamicActorMgrBase::UDynamicActorMgrBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDynamicActorMgrBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnItAll();
	// ...
}



void UDynamicActorMgrBase::SpawnItAll()
{
	TArray<UClass*> HardClasses;
	for (auto& ptr : ClassesToSpawn)
	{
		//ptr.Get()
		HardClasses.Add(ptr.LoadSynchronous());
	}

	int HardClassCount = HardClasses.Num();
	int HardClassIndex = 0;

	for (int i = 0; i < SpawnCount; i++)
	{
		UClass* ClassToSpawn = HardClasses[HardClassIndex];
		HardClassIndex++;
		HardClassIndex = HardClassIndex % HardClassCount;
		if (ClassToSpawn)
		{
			FVector SpawnLocation = FVector(0, 0, 2000);
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnLocation, FRotator::ZeroRotator);
			if (SpawnedActor)
			{
				SpawnedActor->SetActorHiddenInGame(false);
				AllSpawns.Add(SpawnedActor);
			}
		}
	}
}

void UDynamicActorMgrBase::SpawnAllDecals()
{
	//void SpawnBulletHoleDecal(UWorld * World, const FVector & Loc, const FRotator & Rot, UMaterialInterface * Mat)
	//{
	if (!DecalMat) return;

	//UMassEntitySubsystem* MES = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
	//if (!MES) return;

	//FMassEntityManager& EM = MES->GetMutableEntityManager();

	//// Create archetype: decal fragment + lifetime + tag
	//TArray<FInstancedStruct> Fragments;
	//Fragments.Add(FInstancedStruct::Make(FDecalFragment()));
	//Fragments.Add(FInstancedStruct::Make(FDecalLifetimeFragment()));

	//TArray<FInstancedStruct> Tags;
	//Tags.Add(FInstancedStruct::Make(FDecalRequestTag()));
	//
	//FMassArchetypeHandle Archetype = EM.CreateArchetype( ( Fragments, Tags);


	//// Ensure the tag exists so ApplyDecalsProcessor will pick it up
	//EM.AddTagToEntity(E, FDecalRequestTag::StaticStruct());

	//EM.ReserveEntities(Count);

	//TArray<FMassEntityHandle> Entities;
	//Entities.SetNumUninitialized(Count);

	//// Spawn entities in batch
	//EM.BatchCreateEntities(Archetype, Entities);

	//// Assign random decal data
	//for (FMassEntityHandle Entity : Entities)
	//{
	//	FDecalFragment& D = EM.GetFragmentDataChecked<FDecalFragment>(Entity);
	//	FDecalLifetimeFragment& L = EM.GetFragmentDataChecked<FDecalLifetimeFragment>(Entity);

	//	// Random X,Y on plane, Z = 0
	//	D.Location = FVector(FMath::RandRange(-5000.f, 5000.f), FMath::RandRange(-5000.f, 5000.f), 0.f);
	//	D.Rotation = FRotator::ZeroRotator;
	//	D.DecalSize = FVector(64.f, 64.f, 64.f);
	//	D.Material = DecalMat;

	//	L.LifetimeSeconds = 6.f;
	//	L.TimeRemaining = 6.f;
	//	L.PoolIndex = -1; // will be assigned by processor
	//}

}
