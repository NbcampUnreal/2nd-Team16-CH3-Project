#include "Shuriken.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

AShuriken::AShuriken()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(RootComp);

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(RootComp);

    DamageAmount = 20.0f;
    WeaponRange = 10000.0f;
    //Direction = FVector(1.0f, 0.0f, 0.0f);  // �⺻������ X�� �������� �̵�
    Speed = 1000.0f;  // �ӵ� ����

    DefaultFOV = 90.0f; // �⺻ FOV ��
    ZoomedFOV = 60.0f;  // ���� �� FOV ��
    ZoomInterpSpeed = 10.0f;
    bIsAiming = false;
}

void AShuriken::Fire()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController not found!"));
        return;
    }

    // ī�޶� ��ġ �� ���� ��������
    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
    
    // �߻� ������ ī�޶� �������� ����
    Direction = CameraRotation.Vector();

    APawn* Pawn = PlayerController->GetPawn();
    Direction = Pawn->GetActorForwardVector();

    Pose = Pawn->GetActorLocation();
    ActorPose = GetActorLocation();
    Pose.Z = ActorPose.Z;
    EndLocation = Pose + (Direction * WeaponRange);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.AddIgnoredActor(GetOwner());
    QueryParams.bTraceComplex = true;

    FHitResult HitResult;
    bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, EndLocation, ECC_Visibility, QueryParams);
    if (bIsHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            FDamageEvent DamageEvent;
            HitActor->TakeDamage(DamageAmount, DamageEvent, nullptr, this);
            UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s, Applied Damage: %.1f"), *HitActor->GetName(), DamageAmount);
        }

        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 16, FColor::Green, false, 2.0f);
        DrawDebugLine(GetWorld(), CameraLocation, HitResult.ImpactPoint, FColor::Green, false, 2.0f, 0, 1.0f);
    }
    else
    {
        DrawDebugLine(GetWorld(), CameraLocation, EndLocation, FColor::Red, false, 2.0f, 0, 1.0f);
    }
}

void AShuriken::Aim()
{
    // Aim ��� ����
    bIsAiming = true;
}

void AShuriken::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        if (PlayerController->PlayerCameraManager)
        {
            DefaultFOV = PlayerController->PlayerCameraManager->GetFOVAngle();
        }
		APawn* Pawn = PlayerController->GetPawn();
        if (Pawn)
        {
			Direction = Pawn->GetActorForwardVector();
        }
    }
}



void AShuriken::SetAiming(bool bAiming)
{
    bIsAiming = bAiming;
}

// Tick �Լ����� FOV ���� ����
void AShuriken::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    MoveMessage(DeltaTime);
}
void AShuriken::MoveMessage(float DeltaTime)
{
    FVector currentPosition = GetActorLocation();
    FVector newPosition = currentPosition + (Direction * Speed * DeltaTime);

    SetActorLocation(newPosition);
}
