#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include <Kismet/GameplayStatics.h>
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController()
{
    // Behavior Tree 찾기 (블루프린트에서 설정한 AI의 Behavior Tree)
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Game/Blueprints/BT_EnemyAI"));
    if (BT.Succeeded())
    {
        BehaviorTree = BT.Object;
    }

    // Blackboard Component 생성
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

    // AIPerceptionComponent 생성
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

    // 시야 감지 설정
    SightConfig->SightRadius = 1000.0f;
    SightConfig->LoseSightRadius = 1200.0f;
    SightConfig->PeripheralVisionAngleDegrees = 90.0f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

    // AIPerceptionComponent에 시야 감지 설정 적용
    AIPerception->ConfigureSense(*SightConfig);
    AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

    // 감지된 액터가 있을 때 실행될 함수 등록
    if (!AIPerception)
    {
        AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    }

    AIPerception->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("AEnemyAIController: BeginPlay() executed."));

    if (BehaviorTree)
    {
        RunBehaviorTree(BehaviorTree);
        UE_LOG(LogTemp, Warning, TEXT("AI Behavior Tree started successfully."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to start AI Behavior Tree!"));
    }

    // TargetActor 찾기
    TargetActor = FindTargetActorByClass();

    if (TargetActor)
    {
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsObject(TEXT("TargetActor"), TargetActor);
            UE_LOG(LogTemp, Warning, TEXT("TargetActor assigned in Blackboard: %s"), *TargetActor->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("BlackboardComp is NULL in BeginPlay!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No valid TargetActor found! AI has no target."));
    }
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    UE_LOG(LogTemp, Warning, TEXT("AEnemyAIController: OnPossess() executed."));

    if (BehaviorTree)
    {
        if (UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComp))
        {
            RunBehaviorTree(BehaviorTree);
            UE_LOG(LogTemp, Warning, TEXT("EnemyAIController: Behavior Tree started successfully."));

            // 블랙보드에서 TargetActor 가져오기
            AActor* FoundTargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));

            if (FoundTargetActor)
            {
                UE_LOG(LogTemp, Warning, TEXT("OnPossess: TargetActor is OK! Moving to %s"), *FoundTargetActor->GetName());

                //  AI 이동 강제 실행
                MoveToActor(FoundTargetActor, 5.0f, true, true, false, 0, true);

                UE_LOG(LogTemp, Warning, TEXT("MoveToActor executed. AI should be moving."));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("OnPossess: TargetActor is NULL! AI has no valid target."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AEnemyAIController: Failed to initialize Blackboard!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AEnemyAIController: BehaviorTree is NULL!"));
    }
}

AActor* AEnemyAIController::FindTargetActorByClass()
{
    if (!GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("AEnemyAIController: GetWorld() is NULL!"));
        return nullptr;
    }

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        if (Actor->ActorHasTag("Target"))
        {
            UE_LOG(LogTemp, Warning, TEXT("TargetActor found: %s"), *Actor->GetName());
            return Actor;
        }
    }

    UE_LOG(LogTemp, Error, TEXT("No valid TargetActor found! AI has no target."));
    return nullptr;
}

void AEnemyAIController::OnTargetDetected(const TArray<AActor*>& DetectedActors)
{
    if (!BlackboardComp)
    {
        UE_LOG(LogTemp, Error, TEXT("OnTargetDetected: BlackboardComp is NULL!"));
        return;
    }

    if (DetectedActors.Num() > 0)
    {
        AActor* Target = DetectedActors[0];
        if (Target)
        {
            BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Target);
            BlackboardComp->SetValueAsBool(TEXT("IsChasing"), true);
            UE_LOG(LogTemp, Warning, TEXT("OnTargetDetected: AI detected %s and is chasing!"), *Target->GetName());
        }
    }
    else
    {
        BlackboardComp->ClearValue(TEXT("TargetActor"));
        BlackboardComp->SetValueAsBool(TEXT("IsChasing"), false);
        UE_LOG(LogTemp, Warning, TEXT("OnTargetDetected: No target detected, stopping chase."));
    }
}

//void AEnemyAIController::Tick(float DeltaTime)
//{
//    Super::Tick(DeltaTime);
//
//    if (!BlackboardComp)
//    {
//        UE_LOG(LogTemp, Error, TEXT("Tick: BlackboardComp is NULL!"));
//        return;
//    }
//
//    //  블랙보드에서 목표 액터 가져오기 (변수명 변경: TargetActor → FoundTarget)
//    AActor* FoundTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
//
//    if (FoundTarget)
//    {
//        FVector AI_Location = GetPawn()->GetActorLocation();
//        FVector Target_Location = FoundTarget->GetActorLocation();
//
//        float Distance = FVector::Dist(AI_Location, Target_Location);
//
//        UE_LOG(LogTemp, Warning, TEXT("AI Current Location: %s, Distance to Target: %.2f"),
//            *AI_Location.ToString(), Distance);
//
//        if (Distance < 100.0f) // 목표물에 거의 도착했을 때
//        {
//            UE_LOG(LogTemp, Warning, TEXT("AI reached the target!"));
//        }
//    }
//    else
//    {
//        UE_LOG(LogTemp, Error, TEXT("Tick: TargetActor is NULL! AI has no valid target."));
//    }
//}

