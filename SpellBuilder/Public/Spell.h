// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rune.h"
#include "UObject/Object.h"
#include "Spell.generated.h"




#define LOCTEXT_NAMESPACE ""

struct FSpellStatConfig;
enum class ESpellStat : uint8;
class UGameplayEffect;
class URune;
class UElement;

// Formula (Base + Add/Subtracts) * (Multiply/Divide)
USTRUCT(BlueprintType)
struct FSpellStatAggregate
{
	GENERATED_BODY()
		float Base = 0;
	float Multiplier = 1;
	float Divide = 1;

	FSpellStatAggregate(float InBase);
	FSpellStatAggregate();
};

// Certain forms can be chained such as an earth summon that shoots fireballs
// The subspell struct lets us store the information for each part of a spell in a sane manner.
USTRUCT(BlueprintType)
struct FSubSpell
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* SpellIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor SpellColor = FLinearColor(1, 1, 1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText SubSpellName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray < TSubclassOf<URune>> Runes;

	// Notice this is an array. It i
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<UElement>> Elements;

	// For the stone familar part of the subspell. We don't care about the AOE of the fireball
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<ESpellStat, float> Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<UGameplayEffect>> Effects;

	// Actor to fire and forget when we cast
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UObject> SpawnClass = nullptr;

	TArray<ESpellStat> WorkingStats;
	TArray<FSpellStatAggregate> WorkingAggregates;
};



// A spell is a combination of runes
UCLASS(BlueprintType, Blueprintable)
class SPELLBUILDER_API USpell : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText SpellName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray < TSubclassOf<URune>> Runes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSubSpell> SubSpells;

	void CalculateSubspellStats(int x, UForm* FormRune);
	// Generating stats/properties from rune combinations is complicated so best we only do it once
	UFUNCTION(BlueprintCallable)
		void CacheProps();

	void CalculateStats(int x, TArray<ESpellStat>& StatTypes, TArray<FSpellStatConfig>& Configs);

	UFUNCTION(BlueprintCallable)
	bool CanAddRune(TSubclassOf<URune> Rune);

	UFUNCTION(BlueprintCallable)
		bool IsValidSpell();

	UFUNCTION(BlueprintCallable)
		float GetEffectiveRange();

};
