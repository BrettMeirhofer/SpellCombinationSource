// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "UObject/Object.h"
#include "Rune.generated.h"


class URune;
class UForm;

// Formula is (Total of Adds) * ((Total of Multiply) / (Total of Divison))
UENUM(BlueprintType)
enum class EStatMode : uint8
{
	Add, // Add or Subtract stat. Will add stat if stat has not yet been added
	Multiply, // Additive multiplication
	Divide,  // Additive division
	Override,
};

// Simplified stat types for internal use by spells
UENUM(BlueprintType)
enum class ESpellStat : uint8
{
	Default, // Configuring TMaps in BP is annoying if you don't have a throw away first entry like this
	Damage,
	Area,
	ManaCost,
	Bounces,
	Piercing,
	Duration,
	Durability,
	Speed,
	Range,
	Homing,
	HealthCost,
	ManaBurn
};

// Information for how each stat value is configured to combine
USTRUCT(BlueprintType)
struct FSpellStatConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStatMode StatMode = EStatMode::Override;

	FSpellStatConfig();
	FSpellStatConfig(float InAmount, EStatMode InMode);
};

// This struct is used to get around not being able to nest TMap's
USTRUCT(BlueprintType)
struct FSpellStatContainer
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<ESpellStat, FSpellStatConfig> Container = {};
	FSpellStatContainer();
};

inline FSpellStatConfig::FSpellStatConfig(float InAmount, EStatMode InMode)
{
	Amount = InAmount;
	StatMode = InMode;
}

// A rune is a component for a spell
UCLASS(BlueprintType, Blueprintable)
class SPELLBUILDER_API URune : public UObject
{
	GENERATED_BODY()

public:
	// This is used so I can hide unfinished runes from people playing the demo
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Enabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText RuneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText RuneDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* RuneIcon;

	// All the runes in this list must be present in a spell to add this rune
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<URune>> RequiredRunes;

	// If there is anything in this array then atleast one of the options must be in the spell to add this rune
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<URune>> RequiredOR;

	// If any of these runes are already in the spell this rune cannot be added
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<URune>> PreventRunes;

	// Gameplay effects to add to final spell
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<UGameplayEffect>> Effects;

	// Stats to add to spell stack
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<ESpellStat, FSpellStatConfig> Stats;

	// Used when we want rune stats to be different for different forms.
	// Such as speed being additive for summons and multiplicative for projectiles
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TSubclassOf<URune>, FSpellStatContainer> FormSpecificStats;

	// Only used by element runes. Might be used by future addition runes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor Color = FLinearColor(1, 1, 1, 1);
};

// Elements dictate the presentation of a spell and can have unique buffs/debuffs and stat modifiers.
// Fire for example is orange and anything hit with a fire spell gains a burning DOT debuff
UCLASS()
class SPELLBUILDER_API UElement : public URune
{
	GENERATED_BODY()

public:
	UElement();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayTag DamageTag;
};

// Forms dictate the base functionality of a spell such as projectile or wall
UCLASS()
class SPELLBUILDER_API UForm : public URune
{
	GENERATED_BODY()
public:
	UForm();
	// Spawn different actors depending upon element. Used when we want to do more then just change the color for each element
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap < TSubclassOf<URune>, TSubclassOf<UObject>> ClassMap;

	// Many spells are more complicated then spawning self-sufficient actors so each form gets it's own GAS ability.
	// Example: Choosing where to spawn a wall before we spawn the wall.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UGameplayAbility> Ability;

	// Default spawned actor that just gets its color changed dynamically by element
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UObject> DefaultClass;
};

// Additions are any rune that isn't a form or element. They modify existing form + element combinations and can be stacked
// You can put 5 damage runes on a fireball spell to make it deal massive damage but also makes the spell very expensive
UCLASS()
class SPELLBUILDER_API UAddition : public URune
{
	GENERATED_BODY()
public:
	UAddition();
};