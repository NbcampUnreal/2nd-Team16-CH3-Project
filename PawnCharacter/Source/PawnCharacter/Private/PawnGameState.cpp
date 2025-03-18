#include "PawnGameState.h"
#include "PawnGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PawnPlayerController.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"



APawnGameState::APawnGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void APawnGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&APawnGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 APawnGameState::GetScore() const
{
	return Score;
}

void APawnGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
		
	{UPawnGameInstance* PawnGameInstance = Cast<UPawnGameInstance>(GameInstance);
	if (PawnGameInstance)
	{
		PawnGameInstance->AddToScore(Amount);
	}

}

	Score += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}

void APawnGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APawnPlayerController* PawnPlayerController = Cast<APawnPlayerController>(PlayerController))
		{
			PawnPlayerController->ShowGameHUD();
		}
	}


	if (UGameInstance* GameInstance = GetGameInstance())

	{
		UPawnGameInstance* PawnGameInstance = Cast<UPawnGameInstance>(GameInstance);
		if (PawnGameInstance)
		{
			CurrentLevelIndex = PawnGameInstance->CurrentLevelIndex;
		}
	}
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}



	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&APawnGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);
	
}

void APawnGameState::OnLevelTimeUp()
{
		EndLevel();
}

void APawnGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
		CollectedCoinCount, 
		SpawnedCoinCount)

		if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
		{
			EndLevel();
		}
}

void APawnGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())

	{
		UPawnGameInstance* PawnGameInstance = Cast<UPawnGameInstance>(GameInstance);
		if (PawnGameInstance)
		{
			AddScore(Score);	
			CurrentLevelIndex++;
			PawnGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(this, LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void APawnGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APawnPlayerController* PawnPlayerController = Cast<APawnPlayerController>(PlayerController))
		{
			PawnPlayerController->SetPause(true);
			PawnPlayerController->ShowMainMenu(true);
		}
	}
}

void APawnGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APawnPlayerController* PawnPlayerController = Cast<APawnPlayerController>(PlayerController))
			if (UUserWidget* HUDWidget = PawnPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UPawnGameInstance* PawnGameInstance = Cast<UPawnGameInstance>(GameInstance);
						if (PawnGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), PawnGameInstance->TotalScore)));

						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}


			}
	}
}