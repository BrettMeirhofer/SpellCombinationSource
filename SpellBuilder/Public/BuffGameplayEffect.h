// Brett Meirhofer 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "UObject/Object.h"
#include "BuffGameplayEffect.generated.h"

// Use this parent class when you need a gameplayeffect that can appear as an icon on the HUD
UCLASS(Blueprintable, BlueprintType)
class SPELLBUILDER_API UBuffGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Icon;
};
