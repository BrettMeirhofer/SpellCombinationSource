// Brett Meirhofer 2023

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Components/ActorComponent.h"
#include "SpellAbilityComponent.generated.h"


// The base GAS abilitysystemcomponent is bad about exposing functionality to blueprint
// So we use this instead to allow easy customization
class UGameplayAbility;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPELLBUILDER_API USpellAbilityComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USpellAbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// Added on beginplay
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray < TSubclassOf<UGameplayAbility>> DefaultAbilities;

	// Added on beginplay
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;
};
