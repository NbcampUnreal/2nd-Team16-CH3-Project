#include "PawnGameMode.h"
#include "PawnPlayer.h"
#include "PawnGameState.h"

APawnGameMode::APawnGameMode()
{
	GameStateClass = APawnGameState::StaticClass();
	DefaultPawnClass = APawnPlayer::StaticClass();
}

