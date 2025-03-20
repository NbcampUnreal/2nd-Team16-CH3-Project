// Fill out your copyright notice in the Description page of Project Settings.


#include "AEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAEnemyCharacter::AAEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // ĳ���Ͱ� AI ��Ʈ�ѷ��� ���� �ڵ����� �����ǵ��� ����
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    // �̵� ���� ����
    UCharacterMovementComponent* MovementComp = GetCharacterMovement();
    if (MovementComp)
    {
        MovementComp->bUseControllerDesiredRotation = false;  // AI�� ���� ������ ����
        MovementComp->bOrientRotationToMovement = true;  // �̵��ϴ� ������ �ٶ󺸵��� ����
        MovementComp->MaxWalkSpeed = 300.0f;  // �̵� �ӵ� (�⺻ 600, �ʿ�� ����)
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

