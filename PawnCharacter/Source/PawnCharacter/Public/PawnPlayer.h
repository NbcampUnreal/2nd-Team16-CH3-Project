#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Shuriken.h"
#include "Katana.h"
#include "PawnPlayer.generated.h"


class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArm;
class UWidgetComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class PAWNCHARACTER_API APawnPlayer : public APawn
{
    GENERATED_BODY()

public:
    APawnPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);




protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

    UPROPERTY(VisibleAnywhere)
    class UCapsuleComponent* CapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class USkeletalMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    class USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere)
    class UCameraComponent* Camera;

	AShuriken* EquippedShuriken = nullptr;
	AKatana* EquippedKatana = nullptr;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxWalkSpeed = 600.f;

	// Pawn ȸ�� ���� (Yaw)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurnRate = 90.f;

	// ī�޶� ���� ȸ�� ���� (Pitch)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float LookUpRate = 90.f;

	// ī�޶� Pitch ���Ѱ� (�ּ�, �ִ�)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinPitch = -50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxPitch = 90.f;

	// �̵� �� �ӵ� ���� ��� (����/���� ȿ��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float VelocityInterpSpeed = 10.f;

	// ���������� ����Ǵ� �̵� �ӵ� (AnimBP���� Ȱ��)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector Velocity;

    // �̵� �� ȸ�� �Լ�
	FVector2D MovementInput;
	FVector2D LookInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float JumpHeight = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AShuriken* CurrentWeapon;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AKatana* ElseWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AShuriken> DefaultWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AKatana> SecondaryWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName WeaponSocketName2;


	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipShuriken(AShuriken* WeaponToEquip);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void FireWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipKatana(AKatana* WeaponToEquip);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SlashWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount,
		const FDamageEvent& DamageEvent, 
		AController* EventInstigator, 
		AActor* DamageCauser) override;

	void Move(const FInputActionValue& Value);
	void StopMove(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StopLook(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartAiming();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopAiming();

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InputAction_Aim;

	bool bIsAiming;

	void OnDeath();
	void UpdateOverheadHP();
};
