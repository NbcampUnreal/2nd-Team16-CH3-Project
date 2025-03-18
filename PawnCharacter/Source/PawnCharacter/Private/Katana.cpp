#include "Katana.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"



AKatana::AKatana()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(RootComp);

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(RootComp);

    DamageAmount = 20.0f;
    WeaponRange = 50.0f;
}

void AKatana::Slash()
{
    FVector StartLocation = GetOwner()->GetActorLocation();
    FVector ForwardVector = GetOwner()->GetActorForwardVector();
    FVector EndLocation = StartLocation + (ForwardVector * WeaponRange);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.AddIgnoredActor(GetOwner());
    QueryParams.bTraceComplex = true;

    FHitResult HitResult;
    bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
    if (bIsHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            FDamageEvent DamageEvent;
            HitActor->TakeDamage(DamageAmount, DamageEvent, nullptr, this);
            UE_LOG(LogTemp, Warning, TEXT("Slash Actor: %s, Applied Damage: %.1f"), *HitActor->GetName(), DamageAmount);
        }

        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 16, FColor::Blue, false, 2.0f);
        DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Blue, false, 2.0f, 0, 1.0f);
    }
    else
    {
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Yellow, false, 2.0f, 0, 1.0f);
    }
}