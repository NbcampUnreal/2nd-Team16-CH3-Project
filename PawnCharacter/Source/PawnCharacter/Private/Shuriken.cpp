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
    Direction = FVector(1.0f, 0.0f, 0.0f);  // 기본적으로 X축 방향으로 이동
    Speed = 1000.0f;  // 속도 설정

    DefaultFOV = 90.0f; // 기본 FOV 값
    ZoomedFOV = 60.0f;  // 조준 시 FOV 값
    ZoomInterpSpeed = 10.0f;
    bIsAiming = false;
}

void AShuriken::Fire()
{
    APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController not found!"));
        return;
    }

    // 카메라 위치 및 방향 가져오기
    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

    // 발사 방향을 카메라 방향으로 설정
    Direction = CameraRotation.Vector();

    FVector EndLocation = CameraLocation + (Direction * WeaponRange);

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
    // Aim 기능 구현
    bIsAiming = true;
}

void AShuriken::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(GetOwner()))
    {
        if (PlayerController->PlayerCameraManager)
        {
            DefaultFOV = PlayerController->PlayerCameraManager->GetFOVAngle();
        }
    }
}


void AShuriken::SetAiming(bool bAiming)
{
    bIsAiming = bAiming;
}

// Tick 함수에서 FOV 보간 적용
void AShuriken::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    MoveMessage(DeltaTime);
}
void AShuriken::MoveMessage(float DeltaTime)
{
    // 현재 위치 가져오기
    FVector currentPosition = WeaponMesh->GetComponentLocation();

    // 새로운 위치 계산
    FVector newPosition = currentPosition + (Direction * Speed * DeltaTime);

    // 메시의 새로운 위치로 갱신
    WeaponMesh->SetWorldLocation(newPosition);
}
