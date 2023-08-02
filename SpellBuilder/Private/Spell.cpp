// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell.h"
#include "Rune.h"
#include "Kismet/KismetMathLibrary.h"


void USpell::CalculateStats(int x, TArray<ESpellStat>& StatTypes, TArray<FSpellStatConfig>& Configs)
{
	for (int j = 0; j < StatTypes.Num(); j++)
	{
		int Index = SubSpells[x].WorkingStats.Find(StatTypes[j]);
		switch (Configs[j].StatMode)
		{
		case EStatMode::Override:
			SubSpells[x].WorkingStats.Add(StatTypes[j]);
			SubSpells[x].WorkingAggregates.Add(FSpellStatAggregate(Configs[j].Amount));
			break;

		case EStatMode::Add:
			if (Index != -1)
			{
				SubSpells[x].WorkingAggregates[Index].Base += Configs[j].Amount;
			} else
			{
				SubSpells[x].WorkingStats.Add(StatTypes[j]);
				SubSpells[x].WorkingAggregates.Add(FSpellStatAggregate(Configs[j].Amount));
			}
			break;

		case EStatMode::Multiply:
			if (Index != -1)
			{
				SubSpells[x].WorkingAggregates[Index].Multiplier += (Configs[j].Amount - 1);
			}
			break;
		case EStatMode::Divide:
			if (Index != -1)
			{
				SubSpells[x].WorkingAggregates[Index].Divide += (Configs[j].Amount - 1);
			}
			break;
		default:
			break;
		}
	}
}

bool USpell::CanAddRune(TSubclassOf<URune> Rune)
{
	URune* Default = Rune.GetDefaultObject();

	if (SubSpells.Num() == 0)
		SubSpells.Add(FSubSpell());

	FSubSpell& CurrentSubSpell = SubSpells[SubSpells.Num() - 1];

	// Require
	for (int i = 0; i < Default->RequiredRunes.Num(); i++)
	{
		bool Found = false;
		for (int j = 0; j < CurrentSubSpell.Runes.Num(); j++)
		{
			if (UKismetMathLibrary::ClassIsChildOf(CurrentSubSpell.Runes[j], Default->RequiredRunes[i]))
			{
				Found = true;
				break;
			}
		}

		if (!Found)
		{
			return false;
		}
	}

	// Prevent
	for (int i = 0; i < Default->PreventRunes.Num(); i++)
	{
		for (int j = 0; j < CurrentSubSpell.Runes.Num(); j++)
		{
			if (UKismetMathLibrary::ClassIsChildOf(CurrentSubSpell.Runes[j], Default->PreventRunes[i]))
			{
				return false;
			}
		}
	}

	if (Default->RequiredOR.Num() > 0){
		bool Found = false;
		for (int i = 0; i < Default->RequiredOR.Num(); i++)
		{
			if (Default->RequiredOR[i] == nullptr && CurrentSubSpell.Runes.Num() == 0)
			{
				Found = true;
				break;
			}

			for (int j = 0; j < CurrentSubSpell.Runes.Num(); j++)
			{
				if (UKismetMathLibrary::ClassIsChildOf(CurrentSubSpell.Runes[j], Default->RequiredOR[i]))
				{
					Found = true;
					break;
				}
			}
		}

		if (!Found)
			return false;
	}
	

	return true;
}

bool USpell::IsValidSpell()
{
	for (int x = 0; x < SubSpells.Num(); x++)
	{
		UForm* FormRune = nullptr;
		URune* ElementRune = nullptr;
		if (SubSpells[x].Runes.Num() > 0)
		{
			FormRune = Cast<UForm>(SubSpells[x].Runes[0]->ClassDefaultObject);
		}

		if (SubSpells[x].Runes.Num() > 1)
		{
			ElementRune = Cast<URune>(SubSpells[x].Runes[1]->ClassDefaultObject);
		}

		if (!FormRune || !ElementRune)
		{
			return false;
		}
	}

	return true;
}

float USpell::GetEffectiveRange()
{
	float* Range = SubSpells[0].Stats.Find(ESpellStat::Range);
	if (Range) {
		return *Range;
	}
	// TODO: Handle projectiles and walls
	return 1000;
}

FSpellStatAggregate::FSpellStatAggregate(float InBase)
{
	Base = InBase;
}

FSpellStatAggregate::FSpellStatAggregate()
{
}

void USpell::CalculateSubspellStats(int x, UForm* FormRune)
{
	for (int i = 0; i < SubSpells[x].Runes.Num(); i++)
	{
		URune* Rune = Cast<URune>(SubSpells[x].Runes[i]->ClassDefaultObject);
		SubSpells[x].Effects.Append(Rune->Effects);

		TArray<ESpellStat> StatTypes;
		TArray<FSpellStatConfig> Configs;
		Rune->Stats.GenerateKeyArray(StatTypes);
		Rune->Stats.GenerateValueArray(Configs);

		CalculateStats(x, StatTypes, Configs);

		FSpellStatContainer* ContainerPtr = Rune->FormSpecificStats.Find(FormRune->GetClass());
		if (!ContainerPtr)
			continue;
		ContainerPtr->Container.GenerateKeyArray(StatTypes);
		ContainerPtr->Container.GenerateValueArray(Configs);

		CalculateStats(x, StatTypes, Configs);
	}

	for (int j = 0; j < SubSpells[x].WorkingStats.Num(); j++)
	{
		SubSpells[x].Stats.Add(SubSpells[x].WorkingStats[j], SubSpells[x].WorkingAggregates[j].Base * (SubSpells[x].WorkingAggregates[j].Multiplier / SubSpells[x].WorkingAggregates[j].Divide));
	}
}

void USpell::CacheProps()
{
	SubSpells.Empty();
	for (int i = 0; i < Runes.Num(); i++)
	{
		UForm* FormRune = Cast<UForm>(Runes[i]->ClassDefaultObject);
		if (FormRune)
		{
			SubSpells.Add(FSubSpell());
			
		}
		SubSpells[SubSpells.Num() - 1].Runes.Add(Runes[i]);
	}


	
	for (int x = 0; x < SubSpells.Num(); x++)
	{
		UForm* FormRune = nullptr;
		URune* ElementRune = nullptr;
		if (SubSpells[x].Runes.Num() > 0)
		{
			FormRune = Cast<UForm>(SubSpells[x].Runes[0]->ClassDefaultObject);
		}

		if (SubSpells[x].Runes.Num() > 1)
		{
			ElementRune = Cast<URune>(SubSpells[x].Runes[1]->ClassDefaultObject);
		}

		CalculateSubspellStats(x, FormRune);

		SubSpells[x].SpellIcon = nullptr;
		SubSpells[x].SpellColor = FLinearColor::White;
		if (FormRune)
		{
			SubSpells[x].SpellIcon = FormRune->RuneIcon;

			if (ElementRune)
			{
				SubSpells[x].SubSpellName = FText::Format(LOCTEXT("", "{0} {1}"), ElementRune->RuneName, FormRune->RuneName);
				SubSpells[x].SpellColor = ElementRune->Color;

				TSubclassOf<UObject>* Class = FormRune->ClassMap.Find(ElementRune->GetClass());
				if (Class)
				{
					SubSpells[x].SpawnClass = *Class;
				} else
				{
					SubSpells[x].SpawnClass = FormRune->DefaultClass;
				}
			}
		}
	}
}
