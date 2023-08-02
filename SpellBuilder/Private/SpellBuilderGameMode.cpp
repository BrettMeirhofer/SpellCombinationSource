// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpellBuilderGameMode.h"
#include "SpellBuilderCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASpellBuilderGameMode::ASpellBuilderGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
