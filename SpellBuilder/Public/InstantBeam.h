// Brett Meirhofer 2023

#pragma once

#include "CoreMinimal.h"
//#include "NiagaraSystem.h"

#include "UObject/Object.h"
#include "InstantBeam.generated.h"

class UElement;
class UNiagaraComponent;
class UNiagaraSystem;

// Never instantiated only the CDO is used. This lets us have polymorphic behavior for bolt spells without spawning tons of objects
UCLASS(BlueprintType, Blueprintable)
class SPELLBUILDER_API UBolt : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UNiagaraSystem* SpawnSystem;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		UNiagaraComponent* SpawnNiagara(const UObject* WorldContext, FVector Start, FVector End, TSubclassOf<UElement> Element);
};
