// Brett Meirhofer 2023


#include "TraceHelpers.h"

bool UTraceHelpers::LineTraceByObjectChannel(AActor* WorldContext, FHitResult& OutHit, const FVector& Start,
	const FVector& End, ECollisionChannel TraceChannel, const TArray<AActor*> Ignore)
{
	FCollisionQueryParams Params;
	FCollisionResponseParams ResponseParam;
	Params.AddIgnoredActors(Ignore);
	return FPhysicsInterface::RaycastSingle(WorldContext->GetWorld(), OutHit, Start, End, TraceChannel, Params, ResponseParam, FCollisionObjectQueryParams::DefaultObjectQueryParam);
}

UObject* UTraceHelpers::GetClassDefaultObject(TSubclassOf<UObject> Class)
{
	return Class->ClassDefaultObject;
}
