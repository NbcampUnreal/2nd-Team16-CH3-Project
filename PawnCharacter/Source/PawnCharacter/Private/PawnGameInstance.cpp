#include "PawnGameInstance.h"

UPawnGameInstance::UPawnGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UPawnGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;

	UE_LOG(LogTemp, Warning, TEXT("Total Score: %d"), TotalScore);
}