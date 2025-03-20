#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Blueprint/UserWidget.h"
#include "NinjaGameMode.generated.h"

UCLASS()
class PAWNCHARACTER_API ANinjaGameMode : public AGameMode
{
    GENERATED_BODY()

public:
    ANinjaGameMode(); // 생성자

protected:
    virtual void BeginPlay() override; // 게임 시작 시 호출

public:
    // UI 추가 함수
    //void ShowUI(FName UIName);
    //void HideUI(FName UIName);
    //void EndGame(bool bPlayerDead);

private:
    void SpawnEnemies(); // 몬스터 생성 함수
    void StartWave();    // 웨이브 시작 함수

    FTimerHandle EnemySpawnTimer; // 몬스터 스폰 타이머
    FTimerHandle WaveTimer;       // 웨이브 타이머

    int CurrentWave = 0;  // 현재 웨이브
    const int MaxWave = 3; // 최대 웨이브
    const int EnemiesPerWave = 20; // 웨이브당 기본 스폰 수

    TSubclassOf<APawn> EnemyClass; // 블루프린트에서 가져온 몬스터 클래스

    // UI 관리 변수
    //UPROPERTY(EditAnywhere, Category = "UI")
    //TMap<FName, TSubclassOf<UUserWidget>> UIClasses;

    //UPROPERTY()
    //TMap<FName, UUserWidget*> UIInstances;


};
