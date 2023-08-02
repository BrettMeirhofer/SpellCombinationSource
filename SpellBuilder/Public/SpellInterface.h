// Brett Meirhofer 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "UObject/Interface.h"
#include "SpellInterface.generated.h"

class USpell;
enum class ESpellStat : uint8;
// This class does not need to be modified.
UINTERFACE()
class USpellInterface : public UInterface
{
	GENERATED_BODY()
};

// Lets us get generic information regardless of wether our target has a skeletal mesh or is a floating sphere
class SPELLBUILDER_API ISpellInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TeamInterface")
		class USpellComponent* GetSpellComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TeamInterface")
		FVector GetShootLocation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TeamInterface")
		FName GetBeamSocket();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TeamInterface")
		FTransform GetCameraTransform();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TeamInterface")
		USceneComponent* GetSocketComponent();
};
