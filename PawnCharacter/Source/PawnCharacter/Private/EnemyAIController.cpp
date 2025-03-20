#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "UObject/ConstructorHelpers.h"

AEnemyAIController::AEnemyAIController()
{
    // Behavior Tree ã�� (�������Ʈ���� ������ AI�� Behavior Tree)
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Game/AI/BT_EnemyAI"));
    if (BT.Succeeded())
    {
        BehaviorTree = BT.Object;
    }

    // Blackboard Component ����
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
        // ������ ��� ����
        if (UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComp))
        {
            // ������ �� �ʱ�ȭ
            BlackboardComp->SetValueAsObject(TEXT("TargetActor"), nullptr);
            BlackboardComp->SetValueAsVector(TEXT("PatrolLocation"), FVector(0.0f, 0.0f, 0.0f));
            BlackboardComp->SetValueAsBool(TEXT("IsChasing"), false);
            BlackboardComp->SetValueAsFloat(TEXT("Health"), 10.0f);

            // Behavior Tree ����
            RunBehaviorTree(BehaviorTree);
        }
    }
}

