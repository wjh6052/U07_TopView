// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopViewGameMode.h"
#include "TopViewPlayerController.h"
#include "TopViewCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATopViewGameMode::ATopViewGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATopViewPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}