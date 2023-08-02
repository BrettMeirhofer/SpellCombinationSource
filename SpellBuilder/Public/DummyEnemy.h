// Brett Meirhofer 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DummyEnemy.generated.h"

enum class ESpellStat : uint8;
class UAttributeSet;
class UAbilitySystemComponent;

// Simple GAS enabled actor
UCLASS()
class SPELLBUILDER_API ADummyEnemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADummyEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere)
		UAttributeSet* AttributeSet;
};
