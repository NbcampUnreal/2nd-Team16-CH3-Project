// Fill out your copyright notice in the Description page of Project Settings.


#include "AEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAEnemyCharacter::AAEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // 캐릭터가 AI 컨트롤러에 의해 자동으로 조종되도록 설정
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    // 이동 관련 설정
    UCharacterMovementComponent* MovementComp = GetCharacterMovement();
    if (MovementComp)
    {
        MovementComp->bUseControllerDesiredRotation = false;  // AI가 직접 방향을 결정
        MovementComp->bOrientRotationToMovement = true;  // 이동하는 방향을 바라보도록 설정
        MovementComp->MaxWalkSpeed = 300.0f;  // 이동 속도 (기본 600, 필요시 조정)
        MovementComp->NavAgentProps.bCanWalk = true;
        MovementComp->NavAgentProps.bCanJump = false;
        MovementComp->NavAgentProps.bCanSwim = false;
    }

}

// Called when the game starts or when spawned
void AAEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

