// Brett Meirhofer 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "NiagaraComponent.h"
#include "SpellInterface.h"
#include "GameFramework/Actor.h"
#include "SpellBeam.generated.h"


class USpellComponent;
enum class ESpellStat : uint8;

// This is a channeled beam
UCLASS()
class SPELLBUILDER_API ASpellBeam : public AActor, public ISpellInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpellBeam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UNiagaraComponent* BeamEffect;

	float Length;
	float Pierce = 0;

	FTimerHandle DamageTimer;

	// The beam moves on tick but we deal damage much slower then that
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float DamageInterval = .1;

	// The beam movement on tick also finds targets so we save them for the damage function
	UPROPERTY()
	TArray<AActor*> Targets;

	void ApplyDamage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpellComponent* SpellComponent;
	// Called every frame
};
