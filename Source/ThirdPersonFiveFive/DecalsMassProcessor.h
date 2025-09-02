// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "DecalsMassProcessor.generated.h"

/**
 * 
 */
UCLASS()
class UDecalsMassProcessor : public UMassProcessor
{
	GENERATED_BODY()
	
public:
	UDecalsMassProcessor();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery Query;
};
