// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/CoreMisc.h"
#include "InstanceMeshActor.generated.h"

DECLARE_STATS_GROUP(TEXT("TMM"), STATGROUP_TMM, STATCAT_Advanced)

DECLARE_CYCLE_STAT(TEXT("Anim Trace Perf"), STAT_AnimTrace, STATGROUP_TMM)

UCLASS(Blueprintable, BlueprintType)
class THIRDPERSONFIVEFIVE_API AInstanceMeshActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInstanceMeshActor();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* InstanceComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int InstanceCount = 10000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Rotate = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Grow = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* DecalMat = nullptr;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Spheres = true;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnTheDecals();

protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	static TArray<FTransform> RandomTransformsOnSphere(int Count, float Radius);
	TArray<FTransform> RandomTransformsForTraces(int Count, float InnerRadius, float OuterRadius);
	TArray<float> TargetZScale;
	TArray<FTransform> InstanceTransforms;
	TArray<FVector> Directions;

};

