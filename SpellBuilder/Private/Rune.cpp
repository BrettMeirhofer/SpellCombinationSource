// Fill out your copyright notice in the Description page of Project Settings.


#include "Rune.h"

FSpellStatConfig::FSpellStatConfig()
{
}

FSpellStatContainer::FSpellStatContainer()
{
}

UElement::UElement()
{
	// Form needed mixing not allowed
	PreventRunes = { StaticClass() };
	RequiredRunes = { UForm::StaticClass() };
}

UForm::UForm()
{
	// Mixing not allowed
	Stats.Add(ESpellStat::ManaCost, FSpellStatConfig(10, EStatMode::Override));
	RequiredOR = {nullptr};
}

UAddition::UAddition()
{
	RequiredRunes = { UForm::StaticClass(),UElement::StaticClass() };
}
