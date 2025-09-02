// Fill out your copyright notice in the Description page of Project Settings.

#include "DecalsMassProcessor.h"
#include "MassExecutionContext.h"
#include "DecalPoolSubsystem.h"
#include "Engine/World.h"
#include "MassProcessor.h"
#include "DecalFragments.h"
#include "Components/DecalComponent.h"
#include "MassCommonFragments.h"
#include "MassCommonTypes.h"
#include "DecalLifetimeMassProcessor.h"

//#include "MassCommonTypes.h"

UDecalsMassProcessor::UDecalsMassProcessor()
{

	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Tasks;
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Tasks; // or suitable group
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}



void UDecalsMassProcessor::ConfigureQueries()
{
	Query.AddRequirement<FDecalFragment>(EMassFragmentAccess::ReadOnly);
	Query.AddRequirement<FDecalLifetimeFragment>(EMassFragmentAccess::ReadWrite);
	Query.AddTagRequirement<FDecalRequestTag>(EMassFragmentPresence::All);
}

void UDecalsMassProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	if (!Context.GetWorld()) return;

	UDecalPoolSubsystem* Pool = Context.GetWorld()->GetSubsystem<UDecalPoolSubsystem>();
	if (!Pool) return;

	TArrayView<const FDecalFragment> Decals;
	TArrayView<FDecalLifetimeFragment> Lifetimes;

	Query.ForEachEntityChunk(EntityManager, Context, [&](FMassExecutionContext& Ctx)
							 {
								 Decals = Ctx.GetFragmentView<FDecalFragment>();
								 Lifetimes = Ctx.GetMutableFragmentView<FDecalLifetimeFragment>();

								 for (int32 i = 0; i < Ctx.GetNumEntities(); ++i)
								 {
									 const FDecalFragment& D = Decals[i];
									 FDecalLifetimeFragment& L = Lifetimes[i];

									 // Acquire a pool slot if needed
									 if (L.PoolIndex == -1)
									 {
										 const int32 Slot = Pool->Acquire();
										 if (Slot == INDEX_NONE) continue;
										 L.PoolIndex = Slot;
										 L.TimeRemaining = (L.LifetimeSeconds > 0.f) ? L.LifetimeSeconds : 0.f;
									 }

									 if (UDecalComponent* Comp = Pool->Get(L.PoolIndex))
									 {
										 if (D.Material) Comp->SetDecalMaterial(D.Material);
										 Comp->DecalSize = D.DecalSize;

										 FTransform Xf(D.Rotation, D.Location, FVector::OneVector);
										 Comp->SetWorldTransform(Xf);
										 Comp->SetVisibility(true, true);

										 // Optional automatic fadeout if using Lifetime
										 if (L.LifetimeSeconds > 0.f)
										 {
											 Comp->SetFadeOut(L.LifetimeSeconds, 0.25f, false);
										 }
									 }

									 // Remove the tag so we don't re-apply every tick
									 Ctx.Defer().RemoveTag<FDecalRequestTag>(Ctx.GetEntity(i));
								 }
							 });
}
