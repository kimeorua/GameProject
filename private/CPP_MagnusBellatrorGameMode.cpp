// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPP_MagnusBellatrorGameMode.h"
#include "CPP_MagnusBellatrorCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACPP_MagnusBellatrorGameMode::ACPP_MagnusBellatrorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
