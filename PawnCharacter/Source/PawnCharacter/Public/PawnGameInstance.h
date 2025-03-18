#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PawnGameInstance.generated.h"

UCLASS()
class PAWNCHARACTER_API UPawnGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPawnGameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);
};
