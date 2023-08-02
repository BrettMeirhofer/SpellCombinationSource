// Brett Meirhofer 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "SpellInterface.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SpellProjectile.generated.h"

class USphereComponent;
UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExplode);

// Spell projectile that supports homing. Base class for fireballs and windblades
class USpellComponent;
enum class ESpellStat : uint8;
UCLASS()
class SPELLBUILDER_API ASpellProjectile : public AActor, public ISpellInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpellProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Allows for handling messy VFX in BP
	UPROPERTY(BlueprintAssignable)
		FExplode OnExplode;

	// Damage Area
	UPROPERTY(BlueprintReadWrite)
		float Area = 50;

	// Area we search for potential homing targets. Multiplied by homing stat
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HomingArea = 200;

	// How fast we change our velocity to home. Multiplied by homing stat
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HomingAccel = 1;

	UFUNCTION(BlueprintCallable)
		void ExplodeDamage();

	UFUNCTION(BlueprintCallable)
		void BeginOverlap(UPrimitiveComponent* MyComp,AActor* Other, UPrimitiveComponent* OtherComp, int BodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite)
		UPrimitiveComponent* CollisionComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpellComponent* SpellComponent;

	// Not used yet
	UPROPERTY()
		TArray<AActor*> Ignore;

	// Find the closest target. May need to be adjusted so it prefers targets in it's forward cone
	UFUNCTION(BlueprintCallable)
	void HomingSearch();
};

