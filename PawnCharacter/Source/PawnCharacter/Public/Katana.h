#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Katana.generated.h"

UCLASS()
class PAWNCHARACTER_API AKatana : public AActor
{
	GENERATED_BODY()

public:
	AKatana();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Slash();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float WeaponRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* WeaponMesh;
};