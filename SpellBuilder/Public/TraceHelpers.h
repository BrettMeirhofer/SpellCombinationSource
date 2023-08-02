// Brett Meirhofer 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CollisionQueryParams.h"
#include "Engine/HitResult.h"
#include "TraceHelpers.generated.h"

/**
 * 
 */
UCLASS()
class SPELLBUILDER_API UTraceHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Exposed so we can use object channels instead of trace channels
	UFUNCTION(BlueprintCallable)
		static bool LineTraceByObjectChannel(AActor* WorldContext, FHitResult& OutHit, const FVector& Start, const FVector& End, ECollisionChannel TraceChannel, const TArray<AActor*> Ignore);

	// Exposed so we can call functions on class default objects in blueprint
	UFUNCTION(BlueprintCallable)
		static UObject* GetClassDefaultObject(TSubclassOf<UObject> Class);

};
