#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shuriken.generated.h"

UCLASS()
class PAWNCHARACTER_API AShuriken : public AActor
{
    GENERATED_BODY()

public:
    AShuriken();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void Aim();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void Fire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetAiming(bool bAiming);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float DamageAmount;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float WeaponRange;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* RootComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, Category = "Zoom")
    float DefaultFOV;

    UPROPERTY(EditDefaultsOnly, Category = "Zoom")
    float ZoomedFOV;

    UPROPERTY(EditDefaultsOnly, Category = "Zoom")
    float ZoomInterpSpeed;

    // 메시가 날아갈 방향
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    FVector Direction;

    // 메시 이동 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float Speed;

    FVector Pose;
    FVector ActorPose;

    FVector EndLocation;

    // 이동 함수 (헤더에 선언)
    void MoveMessage(float DeltaTime);

    bool bIsAiming;
};
