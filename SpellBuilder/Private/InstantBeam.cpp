// Brett Meirhofer 2023


#include "InstantBeam.h"
#include "NiagaraComponent.h"
#include "Rune.h"
#include "NiagaraFunctionLibrary.h"

UNiagaraComponent* UBolt::SpawnNiagara_Implementation(const UObject* WorldContext, FVector Start, FVector End, TSubclassOf<UElement> Element)
{
	UNiagaraComponent* System = UNiagaraFunctionLibrary::SpawnSystemAtLocation(WorldContext, SpawnSystem, Start);
	System->SetVectorParameter("User.BeamEnd", End);
	System->SetColorParameter("User.BeamColor", Cast<UElement>(Element.GetDefaultObject())->Color);
	return System;
}
