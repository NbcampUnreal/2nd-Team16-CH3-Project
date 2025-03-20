#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "UObject/ConstructorHelpers.h"

AEnemyAIController::AEnemyAIController()
{
    // Behavior Tree 찾기 (블루프린트에서 설정한 AI의 Behavior Tree)
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Game/AI/BT_EnemyAI"));
    if (BT.Succeeded())
    {
        BehaviorTree = BT.Object;
    }

    // Blackboard Component 생성
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (BehaviorTree)
    {
        // 블랙보드 사용 설정
        if (UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComp))
        {
            // 블랙보드 값 초기화
            BlackboardComp->SetValueAsObject(TEXT("TargetActor"), nullptr);
            BlackboardComp->SetValueAsVector(TEXT("PatrolLocation"), FVector(0.0f, 0.0f, 0.0f));
            BlackboardComp->SetValueAsBool(TEXT("IsChasing"), false);
            BlackboardComp->SetValueAsFloat(TEXT("Health"), 10.0f);

            // Behavior Tree 실행
            RunBehaviorTree(BehaviorTree);
        }
    }
}

