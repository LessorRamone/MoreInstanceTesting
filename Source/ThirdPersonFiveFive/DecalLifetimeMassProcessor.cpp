// Fill out your copyright notice in the Description page of Project Settings.


#include "DecalLifetimeMassProcessor.h"
#include "MassCommonFragments.h"
#include "MassCommonTypes.h"
#include "MassExecutionContext.h"
#include "DecalFragments.h"
#include "DecalPoolSubsystem.h"
#include "DecalsMassProcessor.h"

UDecalLifetimeMassProcessor::UDecalLifetimeMassProcessor()
{
ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Tasks;
bAutoRegisterWithProcessingPhases = true;
ExecutionFlags = (int32)EProcessorExecutionFlags::All; 

}

void UDecalLifetimeMassProcessor::ConfigureQueries()
{
	Query.AddRequirement<FDecalLifetimeFragment>(EMassFragmentAccess::ReadWrite);

}

void UDecalLifetimeMassProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	if (!Context.GetWorld()) return;
	UDecalPoolSubsystem* Pool = Context.GetWorld()->GetSubsystem<UDecalPoolSubsystem>();
	if (!Pool) return;

	TArrayView<FDecalLifetimeFragment> Lifetimes;

	const float Dt = Context.GetDeltaTimeSeconds();

	Query.ForEachEntityChunk(EntityManager, Context, [&](FMassExecutionContext& Ctx)
							 {
								 Lifetimes = Ctx.GetMutableFragmentView<FDecalLifetimeFragment>();

								 for (int32 i = 0; i < Ctx.GetNumEntities(); ++i)
								 {
									 FDecalLifetimeFragment& L = Lifetimes[i];

									 if (L.LifetimeSeconds > 0.f && L.PoolIndex != -1)
									 {
										 L.TimeRemaining -= Dt;
										 if (L.TimeRemaining <= 0.f)
										 {
											 Pool->Release(L.PoolIndex);
											 L.PoolIndex = -1;

											 // Optional: destroy entity or mark it reusable
											 // Ctx.Defer().DestroyEntity(Ctx.GetEntity(i));
										 }
									 }
								 }
							 });
}
