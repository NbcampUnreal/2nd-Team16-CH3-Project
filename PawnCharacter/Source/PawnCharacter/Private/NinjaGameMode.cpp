#include "NinjaGameMode.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

ANinjaGameMode::ANinjaGameMode()
{
    // �������Ʈ ���� Ŭ���� �ε� (�� ���� ����)
    static ConstructorHelpers::FClassFinder<APawn> EnemyBP(TEXT("/Game/Blueprints/BP_NormalEnemy"));
    EnemyClass = EnemyBP.Class;
    //UI���÷� ������ UI�߰��� ���⼭ �ݿ�
    //static ConstructorHelpers::FClassFinder<UUserWidget> WaveUI_BP(TEXT("/Game/UI/WaveUI"));
    //static ConstructorHelpers::FClassFinder<UUserWidget> StartUI_BP(TEXT("/Game/UI/StartUI"));
    //static ConstructorHelpers::FClassFinder<UUserWidget> GameOverUI_BP(TEXT("/Game/UI/GameOverUI"));
    //static ConstructorHelpers::FClassFinder<UUserWidget> GoalDestroyedUI_BP(TEXT("/Game/UI/GoalDestroyedUI"));
}

void ANinjaGameMode::BeginPlay()
{
    Super::BeginPlay();


    // ���� ���� UI ǥ��
    //ShowUI("StartUI");

    // 5�ʸ��� ���� ���� (���̺� ���� ������ �ҷ� ����)
    GetWorld()->GetTimerManager().SetTimer(EnemySpawnTimer, this, &ANinjaGameMode::SpawnEnemies, 5.0f, true);

    // 1�� �� ù ��° ���̺� ����
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
//    // ��� UI ����
//    for (auto& UI : UIInstances)
//    {
//        if (UI.Value) UI.Value->SetVisibility(ESlateVisibility::Hidden);
//    }
//
//    if (bPlayerDead)
//    {
//        ShowUI("GameOverUI");
//        UE_LOG(LogTemp, Warning, TEXT("�÷��̾� ���! ���� ����"));
//    }
//    else
//    {
//        ShowUI("GoalDestroyedUI");
//        UE_LOG(LogTemp, Warning, TEXT("��ǥ�� �ı�! ���� ����"));
//    }
//
//    // ���� ���� ����
//    GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
//}


void ANinjaGameMode::StartWave()
{
    //HideUI("StartUI"); // ���̺� ���� �� StartUI �����
    //ShowUI("WaveUI");  // ���̺� UI ǥ��

    if (CurrentWave >= MaxWave)
    {
        UE_LOG(LogTemp, Warning, TEXT("������ ���̺��Դϴ�!"));
        return;
    }

    CurrentWave++;
    UE_LOG(LogTemp, Warning, TEXT("���̺� %d ����!"), CurrentWave);

    // �̹� ���̺꿡�� ������ ���� �� ����
    int SpawnCount = EnemiesPerWave * CurrentWave;

    for (int i = 0; i < SpawnCount; i++)
    {
        SpawnEnemies();
    }

    // ���� ���̺� Ÿ�̸� ���� (3�� �� ���� ���̺�)
    if (CurrentWave < MaxWave)
    {
        GetWorld()->GetTimerManager().SetTimer(WaveTimer, this, &ANinjaGameMode::StartWave, 180.0f, false);
    }
}

void ANinjaGameMode::SpawnEnemies()
{
    if (!EnemyClass)
    {
        UE_LOG(LogTemp, Error, TEXT("EnemyClass�� NULL�̹Ƿ� ���͸� ������ �� �����ϴ�!"));
        return;
    }

    // ������ �� ���� ���� ��ġ ����
    FVector SpawnLocations[2] = {
        FVector(-10880, 7060, 700), // ù ��° ���� ��ġ
        FVector(-29980, 13000, 860)  // �� ��° ���� ��ġ
    };

    // �� ���� ��ġ �߿��� ������ ���鼭 ����
    static int SpawnIndex = 0;
    FVector SpawnLocation = SpawnLocations[SpawnIndex];
    SpawnIndex = (SpawnIndex + 1) % 2; // 0, 1 �����ư��鼭 ����

    FRotator SpawnRotation = FRotator::ZeroRotator;

    // �浹 �����ϵ��� ����
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // ���� ����
    AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotation);

    if (SpawnedEnemy)
    {
        UE_LOG(LogTemp, Warning, TEXT("���Ͱ� �����Ǿ����ϴ�! ��ġ: %s"), *SpawnLocation.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("���� ���� ����!"));
    }
}
