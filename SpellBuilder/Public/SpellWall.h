// Brett Meirhofer 2023

#pragma once

#include "CoreMinimal.h"
#include "DummyEnemy.h"
#include "GameplayEffectTypes.h"
#include "SpellInterface.h"
#include "SpellWall.generated.h"

class USpellComponent;
enum class ESpellStat : uint8;

// The different types of walls have wildly different behaviors so this class might need to be removed
UCLASS()
class SPELLBUILDER_API ASpellWall : public ADummyEnemy, public ISpellInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpellWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpellComponent* SpellComponent;
};
