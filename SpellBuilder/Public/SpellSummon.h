// Brett Meirhofer 2023

#pragma once

#include "CoreMinimal.h"
#include "DummyEnemy.h"
#include "SpellInterface.h"
#include "GameFramework/Actor.h"
#include "SpellSummon.generated.h"

class USpellAbilityComponent;
class USpellComponent;
class USpell;

// Base class for AI controlled allies and enemies
UCLASS()
class SPELLBUILDER_API ASummon : public APawn, public ISpellInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASummon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	AActor* Target;

	FTimerHandle SearchHandle;

	void Search();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpellAbilityComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere)
		UAttributeSet* AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpellComponent* SpellComponent;

	virtual void SpeedChanged(const FOnAttributeChangeData& Data);

	virtual FVector GetShootLocation_Implementation() override;
	virtual FName GetBeamSocket_Implementation() override;
	virtual FTransform GetCameraTransform_Implementation() override;
	virtual USceneComponent* GetSocketComponent_Implementation() override;
};

