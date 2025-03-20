#include "NinjaGameMode.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

ANinjaGameMode::ANinjaGameMode()
{
    // 블루프린트 몬스터 클래스 로드 (한 번만 수행)
    static ConstructorHelpers::FClassFinder<APawn> EnemyBP(TEXT("/Game/Blueprints/BP_NormalEnemy"));
    EnemyClass = EnemyBP.Class;
    //UI예시로 만들어둠 UI추가시 여기서 반영
    //static ConstructorHelpers::FClassFinder<UUserWidget> WaveUI_BP(TEXT("/Game/UI/WaveUI"));
    //static ConstructorHelpers::FClassFinder<UUserWidget> StartUI_BP(TEXT("/Game/UI/StartUI"));
    //static ConstructorHelpers::FClassFinder<UUserWidget> GameOverUI_BP(TEXT("/Game/UI/GameOverUI"));
    //static ConstructorHelpers::FClassFinder<UUserWidget> GoalDestroyedUI_BP(TEXT("/Game/UI/GoalDestroyedUI"));
}

void ANinjaGameMode::BeginPlay()
{
    Super::BeginPlay();


    // 게임 시작 UI 표시
    //ShowUI("StartUI");

    // 5초마다 몬스터 생성 (웨이브 시작 전에도 소량 스폰)
    GetWorld()->GetTimerManager().SetTimer(EnemySpawnTimer, this, &ANinjaGameMode::SpawnEnemies, 5.0f, true);

    // 1분 후 첫 번째 웨이브 시작
    GetWorld()->GetTimerManager().SetTimer(WaveTimer, this, &ANinjaGameMode::StartWave, 60.0f, false);
}

//void ANinjaGameMode::ShowUI(FName UIName)
//{
//    if (UIInstances.Contains(UIName))
//    {
//        UIInstances[UIName]->SetVisibility(ESlateVisibility::Visible);
//        return;
//    }
//
//    if (UIClasses.Contains(UIName))
//    {
//        UUserWidget* NewUI = CreateWidget<UUserWidget>(GetWorld(), UIClasses[UIName]);
//        if (NewUI)
//        {
//            NewUI->AddToViewport();
//            UIInstances.Add(UIName, NewUI);
//        }
//    }
//}
//
//void ANinjaGameMode::HideUI(FName UIName)
//{
//    if (UIInstances.Contains(UIName))
//    {
//        UIInstances[UIName]->SetVisibility(ESlateVisibility::Hidden);
//    }
//}

//void ANinjaGameMode::EndGame(bool bPlayerDead)
//{
//    // 모든 UI 숨김
//    for (auto& UI : UIInstances)
//    {
//        if (UI.Value) UI.Value->SetVisibility(ESlateVisibility::Hidden);
//    }
//
//    if (bPlayerDead)
//    {
//        ShowUI("GameOverUI");
//        UE_LOG(LogTemp, Warning, TEXT("플레이어 사망! 게임 오버"));
//    }
//    else
//    {
//        ShowUI("GoalDestroyedUI");
//        UE_LOG(LogTemp, Warning, TEXT("목표물 파괴! 게임 오버"));
//    }
//
//    // 게임 로직 정지
//    GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
//}


void ANinjaGameMode::StartWave()
{
    //HideUI("StartUI"); // 웨이브 시작 시 StartUI 숨기기
    //ShowUI("WaveUI");  // 웨이브 UI 표시

    if (CurrentWave >= MaxWave)
    {
        UE_LOG(LogTemp, Warning, TEXT("마지막 웨이브입니다!"));
        return;
    }

    CurrentWave++;
    UE_LOG(LogTemp, Warning, TEXT("웨이브 %d 시작!"), CurrentWave);

    // 이번 웨이브에서 스폰할 몬스터 수 증가
    int SpawnCount = EnemiesPerWave * CurrentWave;

    for (int i = 0; i < SpawnCount; i++)
    {
        SpawnEnemies();
    }

    // 다음 웨이브 타이머 설정 (3분 후 다음 웨이브)
    if (CurrentWave < MaxWave)
    {
        GetWorld()->GetTimerManager().SetTimer(WaveTimer, this, &ANinjaGameMode::StartWave, 180.0f, false);
    }
}

void ANinjaGameMode::SpawnEnemies()
{
    if (!EnemyClass)
    {
        UE_LOG(LogTemp, Error, TEXT("EnemyClass가 NULL이므로 몬스터를 생성할 수 없습니다!"));
        return;
    }

    // 고정된 두 개의 스폰 위치 설정
    FVector SpawnLocations[2] = {
        FVector(-10880, 7060, 700), // 첫 번째 스폰 위치
        FVector(-29980, 13000, 860)  // 두 번째 스폰 위치
    };

    // 두 개의 위치 중에서 번갈아 가면서 스폰
    static int SpawnIndex = 0;
    FVector SpawnLocation = SpawnLocations[SpawnIndex];
    SpawnIndex = (SpawnIndex + 1) % 2; // 0, 1 번갈아가면서 변경

    FRotator SpawnRotation = FRotator::ZeroRotator;

    // 충돌 무시하도록 설정
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // 몬스터 생성
    AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotation);

    if (SpawnedEnemy)
    {
        UE_LOG(LogTemp, Warning, TEXT("몬스터가 생성되었습니다! 위치: %s"), *SpawnLocation.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("몬스터 생성 실패!"));
    }
}
