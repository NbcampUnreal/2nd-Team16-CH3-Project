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

    // �޽ð� ���ư� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    FVector Direction;

    // �޽� �̵� �ӵ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float Speed;

    FVector Pose;
    FVector ActorPose;

    FVector EndLocation;

    // �̵� �Լ� (����� ����)
    void MoveMessage(float DeltaTime);

    bool bIsAiming;
};
