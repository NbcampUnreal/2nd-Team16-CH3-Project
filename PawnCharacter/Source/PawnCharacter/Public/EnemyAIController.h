#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyAIController.generated.h"


/**
 * 
 */
UCLASS()
class PAWNCHARACTER_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyAIController();


protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    //virtual void Tick(float DeltaTime) override; // Tick() �Լ� �߰�

private:
    /** Behavior Tree */
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BehaviorTree;

    /** Blackboard Component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    UBlackboardComponent* BlackboardComp;

    //AIPerceptionComponent ����
    UPROPERTY(VisibleAnywhere, Category = "AI")
    UAIPerceptionComponent* AIPerception;

    UPROPERTY()
    UAISenseConfig_Sight* SightConfig;

    /** ������ �÷��̾ ó���ϴ� �Լ� */
    UFUNCTION()
    void OnTargetDetected(const TArray<AActor*>& DetectedActors);

    UPROPERTY(VisibleAnywhere, Category = "AI")
    AActor* TargetActor;  // ��ǥ�� (Ÿ�� �Ǵ� ������)

    /** ��ǥ�� ã�� �Լ� */
    AActor* FindTargetActorByClass();



};
