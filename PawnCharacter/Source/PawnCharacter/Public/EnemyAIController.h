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
    //virtual void Tick(float DeltaTime) override; // Tick() 함수 추가

private:
    /** Behavior Tree */
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BehaviorTree;

    /** Blackboard Component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    UBlackboardComponent* BlackboardComp;

    //AIPerceptionComponent 선언
    UPROPERTY(VisibleAnywhere, Category = "AI")
    UAIPerceptionComponent* AIPerception;

    UPROPERTY()
    UAISenseConfig_Sight* SightConfig;

    /** 감지된 플레이어를 처리하는 함수 */
    UFUNCTION()
    void OnTargetDetected(const TArray<AActor*>& DetectedActors);

    UPROPERTY(VisibleAnywhere, Category = "AI")
    AActor* TargetActor;  // 목표물 (타워 또는 구조물)

    /** 목표물 찾는 함수 */
    AActor* FindTargetActorByClass();



};
