// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonFiveFiveGameMode.h"
#include "ThirdPersonFiveFiveCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThirdPersonFiveFiveGameMode::AThirdPersonFiveFiveGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
