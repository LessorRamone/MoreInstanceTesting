// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DynamicActorMgrBase.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONFIVEFIVE_API UDynamicActorMgrBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDynamicActorMgrBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int SpawnCount = 50;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> DecalMat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftClassPtr<AActor>>  ClassesToSpawn;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void SpawnItAll();
	void SpawnAllDecals();
//
	TArray<AActor*>  AllSpawns;
public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
