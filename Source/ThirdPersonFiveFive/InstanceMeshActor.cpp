// Fill out your copyright notice in the Description page of Project Settings.


#include "InstanceMeshActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/DecalComponent.h"

//DEFINE_STAT(STAT_AnimTrace)

// Sets default values
AInstanceMeshActor::AInstanceMeshActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InstanceComp = Cast<UInstancedStaticMeshComponent>(CreateDefaultSubobject<UInstancedStaticMeshComponent>("InstanceComp"));
	RootComponent = InstanceComp;
}

// Called when the game starts or when spawned
void AInstanceMeshActor::BeginPlay()
{
	if (Spheres)
	{
		InstanceComp->NumCustomDataFloats = 3;
	}
	else
	{
		InstanceComp->NumCustomDataFloats = 6;
	}
	Super::BeginPlay();
	TArray<int> Ids;
	if (Spheres)
	{
		InstanceTransforms = RandomTransformsOnSphere(InstanceCount, Radius);
	}
	else
	{
		InstanceTransforms = RandomTransformsForTraces(InstanceCount, 600.f, Radius );
	}
	Ids = InstanceComp->AddInstances(InstanceTransforms, true, false, false);

	for (int id : Ids)
	{
		if (Spheres)
		{
			InstanceComp->SetCustomData(id, { FMath::FRand(), FMath::FRand(), FMath::FRand() }, false);
		}
		else
		{
			InstanceComp->SetCustomData(id, { FMath::FRand(), FMath::FRand(), FMath::FRand(), (float)Directions[id].X, (float)Directions[id].Y, (float)Directions[id].Z}, false);
		}
	}
	//	SpawnTheDecals();
}

void AInstanceMeshActor::SpawnTheDecals()
{
	for (int i = 0; i < 100000; ++i)
	{
		UDecalComponent* Decal = NewObject<UDecalComponent>(this);
		Decal->RegisterComponent();                 // important
		Decal->SetVisibility(false, true);
		Decal->DecalSize = FVector(64, 64, 64);
		Decal->SetWorldLocation(FVector(FMath::RandRange(-5000.f, 5000.f), FMath::RandRange(-5000.f, 5000.f), 0.f));
		Decal->SetVisibility(false);
		Decal->SetDecalMaterial(DecalMat);
		Decal->SetFadeOut(0.f, 0.f, false);        // no fade by default
		Decal->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	}
}

// Called every frame
void AInstanceMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Rotate)
	{
		const float RotationSpeed = 20.0f; // degrees per second
		FRotator DeltaRotation(0.f, RotationSpeed * DeltaTime, 0.f);
		AddActorLocalRotation(DeltaRotation);
	}
	float Speed = 0.50f; //m/s
	if (Grow)
	{
		{
			SCOPE_CYCLE_COUNTER(STAT_AnimTrace);

#define PAR 0
#if !PAR
			for (int i = 0; i < InstanceCount; ++i)
			{
#else
			ParallelFor(InstanceCount,
						[this, Speed, DeltaTime](int i) {
#endif
							FVector CurrScale = InstanceTransforms[i].GetScale3D();
							float NewScaleZ = FMath::Min(TargetZScale[i], CurrScale.Z + Speed * DeltaTime);
							InstanceTransforms[i].SetScale3D(FVector(CurrScale.X, CurrScale.Y, NewScaleZ));
#if PAR
						});
#else
			}
#endif
		}
		InstanceComp->BatchUpdateInstancesTransforms(0, InstanceTransforms, false, false);
	}
}

TArray<FTransform> AInstanceMeshActor::RandomTransformsOnSphere(int Count, float Radius)
{
	TArray<FTransform> Results;
	for (int i = 0; i < Count; ++i)
	{
		const float Theta = FMath::FRandRange(0.f, 2.f * PI); // Azimuth 
		const float Phi = FMath::Acos(1.f - 2.f * FMath::FRand()); // El

		// Convert spherical coordinates to Cartesian
		const float X = Radius * FMath::Sin(Phi) * FMath::Cos(Theta);
		const float Y = Radius * FMath::Sin(Phi) * FMath::Sin(Theta);
		const float Z = Radius * FMath::Cos(Phi);

		Results.Add(FTransform(FRotator::ZeroRotator, FVector(X, Y, Z), FVector(0.1f)));
	}
	return Results;
}

TArray<FTransform> AInstanceMeshActor::RandomTransformsForTraces(int Count, float InnerRadius, float OuterRadius)
{
	TArray<FTransform> Results;
	Results.Reserve(Count);
	TargetZScale.Reserve(Count);
	Directions.Reserve(Count);
	// should ensure the radii maybe
	float ZScale = Grow ? 1 : (OuterRadius - InnerRadius) / 100.f;
	for (int i = 0; i < Count; ++i)
	{
		const float Theta = FMath::FRandRange(0.f, 2.f * PI); // Azimuth 
		const float Phi = FMath::Acos(1.f - 2.f * FMath::FRand()); // El

		// Convert spherical coordinates to Cartesian
		const float X = FMath::Sin(Phi) * FMath::Cos(Theta);
		const float Y = FMath::Sin(Phi) * FMath::Sin(Theta);
		const float Z = FMath::Cos(Phi);
		FVector Direction(X, Y, Z);
		Directions.Add(Direction);
		FRotationMatrix::MakeFromZ(Direction);
		Results.Add(FTransform(FRotationMatrix::MakeFromZ(Direction).ToQuat(), (InnerRadius * Direction), FVector(0.05f, 0.05f, ZScale)));
		TargetZScale.Add((OuterRadius - InnerRadius) / 100.f);
	}
	return Results;
}
