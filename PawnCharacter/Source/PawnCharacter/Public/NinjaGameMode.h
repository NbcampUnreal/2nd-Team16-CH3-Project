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
    ANinjaGameMode(); // ������

protected:
    virtual void BeginPlay() override; // ���� ���� �� ȣ��

public:
    // UI �߰� �Լ�
    //void ShowUI(FName UIName);
    //void HideUI(FName UIName);
    //void EndGame(bool bPlayerDead);

private:
    void SpawnEnemies(); // ���� ���� �Լ�
    void StartWave();    // ���̺� ���� �Լ�

    FTimerHandle EnemySpawnTimer; // ���� ���� Ÿ�̸�
    FTimerHandle WaveTimer;       // ���̺� Ÿ�̸�

    int CurrentWave = 0;  // ���� ���̺�
    const int MaxWave = 3; // �ִ� ���̺�
    const int EnemiesPerWave = 20; // ���̺�� �⺻ ���� ��

    TSubclassOf<APawn> EnemyClass; // �������Ʈ���� ������ ���� Ŭ����

    // UI ���� ����
    //UPROPERTY(EditAnywhere, Category = "UI")
    //TMap<FName, TSubclassOf<UUserWidget>> UIClasses;

    //UPROPERTY()
    //TMap<FName, UUserWidget*> UIInstances;


};
