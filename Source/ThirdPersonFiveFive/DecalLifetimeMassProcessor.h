// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "DecalLifetimeMassProcessor.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONFIVEFIVE_API UDecalLifetimeMassProcessor : public UMassProcessor
{
	GENERATED_BODY()
public:
	UDecalLifetimeMassProcessor();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery Query;
	
};
