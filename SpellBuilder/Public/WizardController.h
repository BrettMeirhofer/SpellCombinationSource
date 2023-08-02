// Brett Meirhofer 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "WizardController.generated.h"

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateHotbar);

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateKills);

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateExp);

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateLevel);

class USpellComponent;
UCLASS()
class SPELLBUILDER_API AWizardController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWizardController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpellComponent* SpellComponent;

	// Kill total
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Kills;

	// Exp total
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Exp;

	// Required XP for next level
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ExpNeeded = 100;

	// Current Level
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Level = 0;

	// Add stats to player on level up. Currently multiplicative XP progression but flat stat gain
	UFUNCTION(BlueprintCallable)
		void AddExp(float Amount);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void ShowDamageNumber(float Amount, FVector Location, FGameplayTagContainer DamageTags);

	UPROPERTY(BlueprintAssignable)
		FUpdateKills OnUpdateKills;

	UPROPERTY(BlueprintAssignable)
		FUpdateExp OnUpdateExp;

	UPROPERTY(BlueprintAssignable)
		FUpdateHotbar OnUpdateHotbar;

	UPROPERTY(BlueprintAssignable)
		FUpdateLevel OnUpdateLevel;
};
